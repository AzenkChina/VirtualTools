//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#include "communication.h"
#include "include/GXDLMSConverter.h"
#include "include/GXDLMSProfileGeneric.h"
#include "include/GXDLMSDemandRegister.h"
#include "include/GXDLMSTranslator.h"
#include "include/GXDLMSData.h"

void CGXCommunication::WriteValue(GX_TRACE_LEVEL trace, std::string line)
{
    if (trace > GX_TRACE_LEVEL_WARNING)
    {
        printf(line.c_str());
    }
    GXHelpers::Write("LogFile.txt", line);
}


CGXCommunication::CGXCommunication(CGXDLMSSecureClient* pParser, int wt, GX_TRACE_LEVEL trace, char* invocationCounter) :
    m_WaitTime(wt), m_Parser(pParser),
    m_socket(-1), m_Trace(trace), m_InvocationCounter(invocationCounter)
{
#if defined(_WIN32) || defined(_WIN64)//Windows includes
    ZeroMemory(&m_osReader, sizeof(OVERLAPPED));
    ZeroMemory(&m_osWrite, sizeof(OVERLAPPED));
    m_osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
    m_hComPort = INVALID_HANDLE_VALUE;
}

CGXCommunication::~CGXCommunication(void)
{
    Close();
}

//Close connection to the meter.
int CGXCommunication::Disconnect()
{
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1)
    {
        if ((ret = m_Parser->DisconnectRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0)
        {
            //Show error but continue close.
            printf("DisconnectRequest failed (%d) %s.\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
        }
    }
    return 0;
}

//Release connection to the meter.
int CGXCommunication::Release()
{
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1)
    {
        if ((ret = m_Parser->ReleaseRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0)
        {
            //Show error but continue close.
            printf("DisconnectRequest failed (%d) %s.\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
        }
    }
    return 0;
}

//Close connection to the meter and close the communcation channel.
int CGXCommunication::Close()
{
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    /*
    if ((m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1) &&
        (m_Parser->GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER ||
            m_Parser->GetCiphering()->GetSecurity() != DLMS_SECURITY_NONE))
    {
        if ((ret = m_Parser->ReleaseRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0)
        {
            //Show error but continue close.
            printf("ReleaseRequest failed (%d) %s.\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
        }
    }
    */
    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1)
    {
        if ((ret = m_Parser->DisconnectRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0)
        {
            //Show error but continue close.
            printf("DisconnectRequest failed (%d) %s.\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
        }
    }
    if (m_hComPort != INVALID_HANDLE_VALUE)
    {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
        CloseHandle(m_hComPort);
        CloseHandle(m_osReader.hEvent);
        CloseHandle(m_osWrite.hEvent);
#else
        close(m_hComPort);
#endif
        m_hComPort = INVALID_HANDLE_VALUE;
    }
    if (m_socket != -1)
    {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = -1;
    }
    return 0;
}

//Make TCP/IP connection to the meter.
int CGXCommunication::Connect(const char* pAddress, unsigned short Port)
{
    Close();
    //create socket.
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_socket == -1)
    {
        assert(0);
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    sockaddr_in add;
    add.sin_port = htons(Port);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(pAddress);
    //If address is give as name
    if (add.sin_addr.s_addr == INADDR_NONE)
    {
        hostent* Hostent = gethostbyname(pAddress);
        if (Hostent == NULL)
        {
#if defined(_WIN32) || defined(_WIN64)//If Windows
            int err = WSAGetLastError();
#else
            int err = errno;
#endif
            Close();
            return err;
        };
        add.sin_addr = *(in_addr*)(void*)Hostent->h_addr_list[0];
    };

    //Connect to the meter.
    int ret = connect(m_socket, (sockaddr*)&add, sizeof(sockaddr_in));
    if (ret == -1)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    };
    return DLMS_ERROR_CODE_OK;
}

#if defined(_WIN32) || defined(_WIN64)//Windows
int CGXCommunication::GXGetCommState(HANDLE hWnd, LPDCB dcb)
{
    ZeroMemory(dcb, sizeof(DCB));
    dcb->DCBlength = sizeof(DCB);
    if (!GetCommState(hWnd, dcb))
    {
        DWORD err = GetLastError(); //Save occured error.
        if (err == 995)
        {
            COMSTAT comstat;
            unsigned long RecieveErrors;
            if (!ClearCommError(hWnd, &RecieveErrors, &comstat))
            {
                return GetLastError();
            }
            if (!GetCommState(hWnd, dcb))
            {
                return GetLastError(); //Save occured error.
            }
        }
        else
        {
            //If USB to serial port converters do not implement this.
            if (err != ERROR_INVALID_FUNCTION)
            {
                return err;
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::GXSetCommState(HANDLE hWnd, LPDCB DCB)
{
    if (!SetCommState(hWnd, DCB))
    {
        DWORD err = GetLastError(); //Save occured error.
        if (err == 995)
        {
            COMSTAT comstat;
            unsigned long RecieveErrors;
            if (!ClearCommError(hWnd, &RecieveErrors, &comstat))
            {
                return GetLastError();
            }
            if (!SetCommState(hWnd, DCB))
            {
                return GetLastError();
            }
        }
        else
        {
            //If USB to serial port converters do not implement this.
            if (err != ERROR_INVALID_FUNCTION)
            {
                return err;
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

#endif //Windows

int CGXCommunication::Read(unsigned char eop, CGXByteBuffer& reply)
{
#if defined(_WIN32) || defined(_WIN64)//Windows
    unsigned long RecieveErrors;
    COMSTAT comstat;
    DWORD bytesRead = 0;
#else //If Linux.
    unsigned short bytesRead = 0;
    int ret, readTime = 0;
#endif
    int pos;
    unsigned long cnt = 1;
    bool bFound = false;
    int lastReadIndex = reply.GetPosition();
    do
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
        //We do not want to read byte at the time.
        if (!ClearCommError(m_hComPort, &RecieveErrors, &comstat))
        {
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
        bytesRead = 0;
        cnt = 1;
        //Try to read at least one byte.
        if (comstat.cbInQue > 0)
        {
            cnt = comstat.cbInQue;
        }
        //If there is more data than can fit to buffer.
        if (cnt > RECEIVE_BUFFER_SIZE)
        {
            cnt = RECEIVE_BUFFER_SIZE;
        }
        if (!ReadFile(m_hComPort, m_Receivebuff, cnt, &bytesRead, &m_osReader))
        {
            DWORD nErr = GetLastError();
            if (nErr != ERROR_IO_PENDING)
            {
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            //Wait until data is actually read
            if (::WaitForSingleObject(m_osReader.hEvent, m_WaitTime) != WAIT_OBJECT_0)
            {
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            if (!GetOverlappedResult(m_hComPort, &m_osReader, &bytesRead, TRUE))
            {
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
        }
#else
        //Get bytes available.
        ret = ioctl(m_hComPort, FIONREAD, &cnt);
        //If driver is not supporting this functionality.
        if (ret < 0)
        {
            cnt = RECEIVE_BUFFER_SIZE;
        }
        else if (cnt == 0)
        {
            //Try to read at least one byte.
            cnt = 1;
        }
        //If there is more data than can fit to buffer.
        if (cnt > RECEIVE_BUFFER_SIZE)
        {
            cnt = RECEIVE_BUFFER_SIZE;
        }
        bytesRead = read(m_hComPort, m_Receivebuff, cnt);
        if (bytesRead == 0xFFFF)
        {
            //If wait time has elapsed.
            if (errno == EAGAIN)
            {
                if (readTime > m_WaitTime)
                {
                    printf("Read failed. Timeout occurred.\r\n");
                    return DLMS_ERROR_CODE_RECEIVE_FAILED;
                }
                readTime += 100;
                bytesRead = 0;
            }
            //If connection is closed.
            else if (errno == EBADF)
            {
                printf("Read failed. Connection closed.\r\n");
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            else
            {
                printf("Read failed. %d.\r\n", errno);
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
        }
#endif
        reply.Set(m_Receivebuff, bytesRead);
        //Note! Some USB converters can return true for ReadFile and Zero as bytesRead.
        //In that case wait for a while and read again.
        if (bytesRead == 0)
        {
#if defined(_WIN32) || defined(_WIN64)//Windows
            Sleep(100);
#else
            usleep(100000);
#endif
            continue;
        }
        if (reply.GetSize() > 5)
        {
            //Some optical strobes can return extra bytes.
            for (pos = reply.GetSize() - 1; pos != lastReadIndex; --pos)
            {
                if (reply.GetData()[pos] == eop)
                {
                    bFound = true;
                    break;
                }
            }
            lastReadIndex = pos;
        }
    } while (!bFound);
    return DLMS_ERROR_CODE_OK;
}

//Open serial port.
int CGXCommunication::Open(const char* settings, bool iec, int maxBaudrate)
{
    Close();
    unsigned short baudRate;
#if defined(_WIN32) || defined(_WIN64)
    unsigned char parity;
#else //Linux
    int parity;
#endif
    unsigned char stopBits, dataBits = 8;
    std::string port;
    port = settings;
    std::vector< std::string > tmp = GXHelpers::Split(port, ':');
    std::string tmp2;
    port.clear();
    port = tmp[0];
    if (tmp.size() > 1)
    {
        baudRate = atoi(tmp[1].c_str());
        dataBits = atoi(tmp[2].substr(0, 1).c_str());
        tmp2 = tmp[2].substr(1, tmp[2].size() - 2);
        if (tmp2.compare("None") == 0)
        {
#if defined(_WIN32) || defined(_WIN64)
            parity = NOPARITY;
#else //Linux
            parity = 0;
#endif
        }
        else if (tmp2.compare("Odd") == 0)
        {
#if defined(_WIN32) || defined(_WIN64)
            parity = ODDPARITY;
#else //Linux
            parity = PARENB | PARODD;
#endif
        }
        else if (tmp2.compare("Even") == 0)
        {
#if defined(_WIN32) || defined(_WIN64)
            parity = EVENPARITY;
#else //Linux
            parity = PARENB | PARENB;
#endif
        }
        else if (tmp2.compare("Mark") == 0)
        {
#if defined(_WIN32) || defined(_WIN64)
            parity = MARKPARITY;
#else //Linux
            parity = PARENB | PARODD | CMSPAR;
#endif
        }
        else if (tmp2.compare("Space") == 0)
        {
#if defined(_WIN32) || defined(_WIN64)
            parity = SPACEPARITY;
#else //Linux
            parity = PARENB | CMSPAR;
#endif
        }
        else
        {
            printf("Invalid parity :\"%s\"\r\n", tmp2.c_str());
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        stopBits = atoi(tmp[2].substr(tmp[2].size() - 1, 1).c_str());
    }
    else
    {
#if defined(_WIN32) || defined(_WIN64)
        baudRate = 9600;
        parity = NOPARITY;
        stopBits = ONESTOPBIT;
#else
        baudRate = B9600;
        parity = 0;
        stopBits = 0;
#endif
        dataBits = 8;
    }

    CGXByteBuffer reply;
    int ret, len, pos;
    unsigned char ch;
    //In Linux serial port name might be very long.
    char buff[50];
#if defined(_WIN32) || defined(_WIN64)
    DCB dcb = { 0 };
    unsigned long sendSize = 0;
#if _MSC_VER > 1000
    sprintf_s(buff, 50, "\\\\.\\%s", port.c_str());
#else
    sprintf(buff, "\\\\.\\%s", port.c_str());
#endif
    //Open serial port for read / write. Port can't share.
    m_hComPort = CreateFileA(buff,
        GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (m_hComPort == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open serial port: \"%s\"\r\n", port.c_str());
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    dcb.DCBlength = sizeof(DCB);
    if ((ret = GXGetCommState(m_hComPort, &dcb)) != 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    dcb.fBinary = 1;
    dcb.fOutX = dcb.fInX = 0;
    //Abort all reads and writes on Error.
    dcb.fAbortOnError = 1;
    if (iec)
    {
        dcb.BaudRate = 300;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = EVENPARITY;
        dcb.ByteSize = 7;
    }
    else
    {
        dcb.BaudRate = baudRate;
        dcb.ByteSize = dataBits;
        dcb.StopBits = stopBits;
        dcb.Parity = parity;
    }
    if(!SetupComm(m_hComPort, 2048, 2048))
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    if(!SetCommTimeouts(m_hComPort, &timeouts))
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if ((ret = GXSetCommState(m_hComPort, &dcb)) != 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    if(!PurgeComm(m_hComPort, PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_TXABORT))
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
#else //#if defined(__LINUX__)
    struct termios options;
    // read/write | not controlling term | don't wait for DCD line signal.
    m_hComPort = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (m_hComPort == -1) // if open is unsuccessful.
    {
        printf("Failed to open serial port: \"%s\"\r\n", port.c_str());
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    else
    {
        if (!isatty(m_hComPort))
        {
            printf("Failed to Open port. This is not a serial port.\r\n");
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        memset(&options, 0, sizeof(options));
        options.c_iflag = 0;
        options.c_oflag = 0;
        if (iec)
        {
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag &= ~CSTOPB;
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS7;
            //Set Baud Rates
            cfsetospeed(&options, B300);
            cfsetispeed(&options, B300);
        }
        else
        {
            // 8n1, see termios.h for more information
            options.c_cflag = CS8 | CREAD | CLOCAL;
            options.c_cflag |= parity;
            /*
            options.c_cflag &= ~PARENB
            options.c_cflag &= ~CSTOPB
            options.c_cflag &= ~CSIZE;
            options.c_cflag |= CS8;
            */
            //Set Baud Rates
            cfsetospeed(&options, B9600);
            cfsetispeed(&options, B9600);
        }
        options.c_lflag = 0;
        options.c_cc[VMIN] = 1;
        //How long we are waiting reply charachter from serial port.
        options.c_cc[VTIME] = m_WaitTime / 1000;
        //hardware flow control is used as default.
        //options.c_cflag |= CRTSCTS;
        if (tcsetattr(m_hComPort, TCSAFLUSH, &options) != 0)
        {
            printf("Failed to Open port. tcsetattr failed.\r\n");
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
#endif
    if (iec)
    {
#if _MSC_VER > 1000
        strcpy_s(buff, 50, "/?!\r\n");
#else
        strcpy(buff, "/?!\r\n");
#endif
        len = (int)strlen(buff);
        if (m_Trace > GX_TRACE_LEVEL_WARNING)
        {
            printf("\r\nTX:\t");
            for (pos = 0; pos != len; ++pos)
            {
                printf("%.2X ", buff[pos]);
            }
            printf("\r\n");
        }
#if defined(_WIN32) || defined(_WIN64)
        ret = WriteFile(m_hComPort, buff, len, &sendSize, &m_osWrite);
        if (ret == 0)
        {
            DWORD err = GetLastError();
            //If error occurs...
            if (err != ERROR_IO_PENDING)
            {
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            //Wait until data is actually sent
            WaitForSingleObject(m_osWrite.hEvent, INFINITE);
        }
#else //#if defined(__LINUX__)
        ret = write(m_hComPort, buff, len);
        if (ret != len)
        {
            printf("write failed %d\r\n", errno);
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
#endif
        //Read reply data.
        if (Read('\n', reply) != 0)
        {
            return DLMS_ERROR_CODE_RECEIVE_FAILED;
        }
        //Remove echo.
        if (reply.Compare((unsigned char*)buff, len))
        {
            if (Read('\n', reply) != 0)
            {
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
        }

        if (m_Trace > GX_TRACE_LEVEL_WARNING)
        {
            printf("RX:\t%s\r\n", reply.ToHexString().c_str());
        }
        if (reply.GetUInt8(&ch) != 0 || ch != '/')
        {
            //Send disc
            buff[0] = 1;
            buff[1] = 'B';
            buff[2] = '0';
            buff[3] = 3;
            buff[4] = '\r';
            buff[5] = '\n';
            len = 6;
#if defined(_WIN32) || defined(_WIN64)
            ret = WriteFile(m_hComPort, buff, len, &sendSize, &m_osWrite);
            if (ret == 0)
            {
                DWORD err = GetLastError();
                //If error occurs...
                if (err != ERROR_IO_PENDING)
                {
                    return DLMS_ERROR_CODE_SEND_FAILED;
                }
                //Wait until data is actually sent
                WaitForSingleObject(m_osWrite.hEvent, INFINITE);
            }
#else //#if defined(__LINUX__)
            ret = write(m_hComPort, buff, len);
            if (ret != len)
            {
                printf("write failed %d\r\n", errno);
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
#endif
            if (Read('\n', reply) != 0)
            {
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
        //Get used baud rate.
        if ((ret = reply.GetUInt8(reply.GetPosition() + 3, &ch)) != 0)
        {
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
        switch (ch)
        {
        case '0':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 300;
#else
            baudRate = B300;
#endif
            break;
        case '1':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 600;
#else
            baudRate = B600;
#endif
            break;
        case '2':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 1200;
#else
            baudRate = B1200;
#endif
            break;
        case '3':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 2400;
#else
            baudRate = B2400;
#endif
            break;
        case '4':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 4800;
#else
            baudRate = B4800;
#endif
            break;
        case '5':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 9600;
#else
            baudRate = B9600;
#endif
            break;
        case '6':
#if defined(_WIN32) || defined(_WIN64)
            baudRate = 19200;
#else
            baudRate = B19200;
#endif
            break;
        default:
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        //Send ACK
        buff[0] = 0x06;
        //Send Protocol control character
        buff[1] = '2';// "2" HDLC protocol procedure (Mode E)
        buff[2] = (char)ch;
        buff[3] = '2';
        buff[4] = (char)0x0D;
        buff[5] = 0x0A;
        len = 6;
        reply.Clear();
        if (m_Trace > GX_TRACE_LEVEL_WARNING)
        {
            printf("\r\nTX: ");
            for (pos = 0; pos != len; ++pos)
            {
                printf("%.2X ", buff[pos]);
            }
            printf("\r\n");
        }
#if defined(_WIN32) || defined(_WIN64)//Windows
        ret = WriteFile(m_hComPort, buff, len, &sendSize, &m_osWrite);
        if (ret == 0)
        {
            int err = GetLastError();
            //If error occurs...
            if (err != ERROR_IO_PENDING)
            {
                printf("WriteFile %d\r\n", err);
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            //Wait until data is actually sent
            WaitForSingleObject(m_osWrite.hEvent, INFINITE);
        }
#else //#if defined(__LINUX__)
        ret = write(m_hComPort, buff, len);
        if (ret != len)
        {
            printf("write failed %dr\n", errno);
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
#endif
#if defined(_WIN32) || defined(_WIN64)//Windows
        //Some meters need this sleep. Do not remove.
        Sleep(200);
        dcb.BaudRate = baudRate;
        printf("New baudrate %d\r\n", (int)dcb.BaudRate);
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;
        if ((ret = GXSetCommState(m_hComPort, &dcb)) != 0)
        {
            printf("GXSetCommState failed %d\n", ret);
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        //Some meters need this sleep. Do not remove.
        Sleep(800);
#else
        //Some meters need this sleep. Do not remove.
        usleep(200000);
        // 8n1, see termios.h for more information
        options.c_cflag = CS8 | CREAD | CLOCAL;
        //Set Baud Rates
        cfsetospeed(&options, baudRate);
        cfsetispeed(&options, baudRate);
        if (tcsetattr(m_hComPort, TCSAFLUSH, &options) != 0)
        {
            printf("Failed to Open port. tcsetattr failed %d.\r\n", errno);
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        //Some meters need this sleep. Do not remove.
        usleep(800000);
#endif
        //It's ok if this fails.
        Read('\n', reply);
    }
    return DLMS_ERROR_CODE_OK;
}

/// Read Invocation counter (frame counter) from the meter and update it.
int CGXCommunication::UpdateFrameCounter()
{
    int ret = 0;
    //Read frame counter if GeneralProtection is used.
    if (m_InvocationCounter != NULL && m_Parser->GetCiphering() != NULL && m_Parser->GetCiphering()->GetSecurity() != DLMS_SECURITY_NONE)
    {
        if (m_Trace > GX_TRACE_LEVEL_WARNING)
        {
            printf("UpdateFrameCounter\r\n");
        }
        m_Parser->SetProposedConformance((DLMS_CONFORMANCE)(m_Parser->GetProposedConformance() | DLMS_CONFORMANCE_GENERAL_PROTECTION));
        unsigned long add = m_Parser->GetClientAddress();
        DLMS_AUTHENTICATION auth = m_Parser->GetAuthentication();
        DLMS_SECURITY security = m_Parser->GetCiphering()->GetSecurity();
        CGXByteBuffer challenge = m_Parser->GetCtoSChallenge();
        std::vector<CGXByteBuffer> data;
        CGXReplyData reply;
        m_Parser->SetClientAddress(16);
        m_Parser->SetAuthentication(DLMS_AUTHENTICATION_NONE);
        m_Parser->GetCiphering()->SetSecurity(DLMS_SECURITY_NONE);
        //Get meter's send and receive buffers size.
        if ((ret = m_Parser->SNRMRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseUAResponse(reply.GetData())) != 0)
        {
            printf("SNRMRequest failed %d.\r\n", ret);
            return ret;
        }
        reply.Clear();
        if ((ret = m_Parser->AARQRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseAAREResponse(reply.GetData())) != 0)
        {
            if (ret == DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED)
            {
                printf("Use Logical Name referencing is wrong. Change it!\r\n");
                return ret;
            }
            printf("AARQRequest failed (%d) %s\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
            return ret;
        }
        reply.Clear();
        std::string ln;
        ln.append(m_InvocationCounter);
        CGXDLMSData d(ln);
        std::string str;
        if ((ret = Read(&d, 2, str)) == 0)
        {
            m_Parser->GetCiphering()->SetInvocationCounter(1 + d.GetValue().ToInteger());
        }
        printf("Invocation counter: %d\r\n", m_Parser->GetCiphering()->GetInvocationCounter());
        reply.Clear();
        Disconnect();
        m_Parser->SetClientAddress(add);
        m_Parser->SetAuthentication(auth);
        m_Parser->GetCiphering()->SetSecurity(security);
        m_Parser->SetCtoSChallenge(challenge);
    }
    return ret;
}


//Initialize connection to the meter.
int CGXCommunication::InitializeConnection()
{
    int ret = 0;
    if ((ret = UpdateFrameCounter()) != 0)
    {
        return ret;
    }
    if (m_Trace > GX_TRACE_LEVEL_WARNING)
    {
        printf("InitializeConnection\r\n");
    }
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->SNRMRequest(data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->ParseUAResponse(reply.GetData())) != 0)
    {
        printf("SNRMRequest failed %d.\r\n", ret);
        return ret;
    }
    reply.Clear();
    if ((ret = m_Parser->AARQRequest(data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->ParseAAREResponse(reply.GetData())) != 0)
    {
        if (ret == DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED)
        {
            printf("Use Logical Name referencing is wrong. Change it!\r\n");
            return ret;
        }
        printf("AARQRequest failed (%d) %s\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
        return ret;
    }
    reply.Clear();
    // Get challenge Is HLS authentication is used.
    if (m_Parser->IsAuthenticationRequired())
    {
        if ((ret = m_Parser->GetApplicationAssociationRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseApplicationAssociationResponse(reply.GetData())) != 0)
        {
            printf("Authentication failed (%d) %s\r\n", ret, CGXDLMSConverter::GetErrorMessage(ret));
            return ret;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

// Read DLMS Data frame from the device.
int CGXCommunication::ReadDLMSPacket(CGXByteBuffer& data, CGXReplyData& reply)
{
    int ret;
    CGXByteBuffer bb;
    std::string tmp;
    CGXReplyData notify;
    if (data.GetSize() == 0)
    {
        return DLMS_ERROR_CODE_OK;
    }
    Now(tmp);
    tmp = "TX:\t" + tmp;
    tmp += "\t" + data.ToHexString();
    if (m_Trace > GX_TRACE_LEVEL_INFO)
    {
        printf("%s\r\n", tmp.c_str());
    }
    GXHelpers::Write("trace.txt", tmp + "\r\n");
    int len = data.GetSize();
    if (m_hComPort != INVALID_HANDLE_VALUE)
    {
#if defined(_WIN32) || defined(_WIN64)//If Windows
        DWORD sendSize = 0;
        BOOL bRes = ::WriteFile(m_hComPort, data.GetData(), len, &sendSize, &m_osWrite);
        if (!bRes)
        {
            COMSTAT comstat;
            unsigned long RecieveErrors;
            DWORD err = GetLastError();
            //If error occurs...
            if (err != ERROR_IO_PENDING)
            {
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            //Wait until data is actually sent
            ret = WaitForSingleObject(m_osWrite.hEvent, m_WaitTime);
            if (ret != 0)
            {
                DWORD err = GetLastError();
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            //Read bytes in output buffer. Some USB converts require this.
            if (!ClearCommError(m_hComPort, &RecieveErrors, &comstat))
            {
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            //Discards old data in the rx buffer.
            if(!PurgeComm(m_hComPort, PURGE_RXCLEAR | PURGE_RXABORT))
            {
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
        }
#else //If Linux
        ret = write(m_hComPort, data.GetData(), len);
        if (ret != len)
        {
            printf("write failed %d\n", errno);
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
        //Discards old data in the rx buffer.
        tcflush(fd, TCIFLUSH);
#endif
    }
    else if ((ret = send(m_socket, (const char*)data.GetData(), len, 0)) == -1)
    {
        //If error has occured
#if defined(_WIN32) || defined(_WIN64)//If Windows
        printf("send failed %d\n", WSAGetLastError());
#else
        printf("send failed %d\n", errno);
#endif
        return DLMS_ERROR_CODE_SEND_FAILED;
    }
    // Loop until whole DLMS packet is received.
    tmp = "";
    do
    {
        if (notify.GetData().GetSize() != 0)
        {
            //Handle notify.
            if (!notify.IsMoreData())
            {
                //Show received push message as XML.
                std::string xml;
                CGXDLMSTranslator t(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML);
                if ((ret = t.DataToXml(notify.GetData(), xml)) != 0)
                {
                    printf("ERROR! DataToXml failed.");
                }
                else
                {
                    printf("%s\r\n", xml.c_str());
                }
                notify.Clear();
            }
            continue;
        }

        if (m_hComPort != INVALID_HANDLE_VALUE)
        {
            unsigned short pos = (unsigned short)bb.GetSize();
            if (Read(0x7E, bb) != 0)
            {
                tmp += bb.ToHexString(pos, bb.GetSize() - pos, true);
                printf("Read failed.\r\n%s", tmp.c_str());
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            if (tmp.size() == 0)
            {
                Now(tmp);
                tmp = "RX:\t" + tmp + "\t";
            }
            else
            {
                tmp += " ";
            }
            tmp += bb.ToHexString(pos, bb.GetSize() - pos, true);
        }
        else
        {
            len = RECEIVE_BUFFER_SIZE;
            if ((ret = recv(m_socket, (char*)m_Receivebuff, len, 0)) == -1)
            {
#if defined(_WIN32) || defined(_WIN64)//If Windows
                printf("recv failed %d\n", WSAGetLastError());
#else
                printf("recv failed %d\n", errno);
#endif
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            bb.Set(m_Receivebuff, ret);
            if (tmp.size() == 0)
            {
                Now(tmp);
                tmp = "RX:\t" + tmp + "\t";
            }
            else
            {
                tmp += " ";
            }
            tmp += GXHelpers::BytesToHex(m_Receivebuff, ret);
        }
    } while ((ret = m_Parser->GetData(bb, reply, notify)) == DLMS_ERROR_CODE_FALSE);
    tmp += "\r\n";
    if (m_Trace > GX_TRACE_LEVEL_INFO)
    {
        printf("%s", tmp.c_str());
    }
    GXHelpers::Write("trace.txt", tmp);
    if (ret == DLMS_ERROR_CODE_REJECTED)
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
        Sleep(1000);
#else
        usleep(1000000);
#endif
        ret = ReadDLMSPacket(data, reply);
    }
    return ret;
}

int CGXCommunication::ReadDataBlock(CGXByteBuffer& data, CGXReplyData& reply)
{
    //If ther is no data to send.
    if (data.GetSize() == 0)
    {
        return DLMS_ERROR_CODE_OK;
    }
    int ret;
    CGXByteBuffer bb;
    //Send data.
    if ((ret = ReadDLMSPacket(data, reply)) != DLMS_ERROR_CODE_OK)
    {
        return ret;
    }
    while (reply.IsMoreData())
    {
        bb.Clear();
        if ((ret = m_Parser->ReceiverReady(reply.GetMoreData(), bb)) != 0)
        {
            return ret;
        }
        if ((ret = ReadDLMSPacket(bb, reply)) != DLMS_ERROR_CODE_OK)
        {
            return ret;
        }
    }
    return DLMS_ERROR_CODE_OK;
}


int CGXCommunication::ReadDataBlock(std::vector<CGXByteBuffer>& data, CGXReplyData& reply)
{
    //If ther is no data to send.
    if (data.size() == 0)
    {
        return DLMS_ERROR_CODE_OK;
    }
    int ret;
    CGXByteBuffer bb;
    //Send data.
    for (std::vector<CGXByteBuffer>::iterator it = data.begin(); it != data.end(); ++it)
    {
        //Send data.
        if ((ret = ReadDLMSPacket(*it, reply)) != DLMS_ERROR_CODE_OK)
        {
            return ret;
        }
        while (reply.IsMoreData())
        {
            bb.Clear();
            if ((ret = m_Parser->ReceiverReady(reply.GetMoreData(), bb)) != 0)
            {
                return ret;
            }
            if ((ret = ReadDLMSPacket(bb, reply)) != DLMS_ERROR_CODE_OK)
            {
                return ret;
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

//Read selected object.
int CGXCommunication::Read(CGXDLMSObject* pObject, int attributeIndex, std::string& value)
{
    value.clear();
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    //Read data from the meter.
    if ((ret = m_Parser->Read(pObject, attributeIndex, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->UpdateValue(*pObject, attributeIndex, reply.GetValue())) != 0)
    {
        return ret;
    }

    value = reply.GetData().ToString();
    return DLMS_ERROR_CODE_OK;
}

//Write selected object.
int CGXCommunication::Write(CGXDLMSObject* pObject, int attributeIndex, CGXByteBuffer& value)
{
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Write(pObject, attributeIndex, value, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0)
    {
        return ret;
    }
    return DLMS_ERROR_CODE_OK;
}

//Write selected object.
int CGXCommunication::Write(CGXDLMSObject* pObject, int attributeIndex, CGXByteBuffer& value, CGXReplyData & reply)
{
    int ret;
    std::vector<CGXByteBuffer> data;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Write(pObject, attributeIndex, value, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0)
    {
        return ret;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Method(CGXDLMSObject* pObject, int attributeIndex, CGXByteBuffer& value)
{
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Method(pObject, attributeIndex, value, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0)
    {
        return ret;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Method(CGXDLMSObject* pObject, int attributeIndex, CGXByteBuffer& value, CGXReplyData & reply)
{
    int ret;
    std::vector<CGXByteBuffer> data;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Method(pObject, attributeIndex, value, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0)
    {
        return ret;
    }
    return DLMS_ERROR_CODE_OK;
}
