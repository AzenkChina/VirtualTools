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

#include "../include/GXDLMSCommon.h"

//SN Constructor.
CGXDLMSCommon::CGXDLMSCommon(unsigned short type, std::string ln) :
    CGXDLMSObject(static_cast<DLMS_OBJECT_TYPE>(type), ln, 0)
{

}


// Get value of COSEM Data object.
CGXDLMSVariant&CGXDLMSCommon::GetValue()
{
    return m_Value;
}

// Set value of COSEM Data object.
void CGXDLMSCommon::SetValue(CGXDLMSVariant& value)
{
    m_Value = value;
}

// Returns amount of attributes.
int CGXDLMSCommon::GetAttributeCount()
{
    return 32;
}

// Returns amount of methods.
int CGXDLMSCommon::GetMethodCount()
{
    return 16;
}

void CGXDLMSCommon::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_Value.ToString());
}

void CGXDLMSCommon::GetAttributeIndexToRead(bool all, std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (all || CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
}

int CGXDLMSCommon::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    else
    {
		type = DLMS_DATA_TYPE_NONE;
        return DLMS_ERROR_CODE_OK;
    }
}

// Returns value of given attribute.
int CGXDLMSCommon::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    else
    {
        e.SetValue(m_Value);
        return DLMS_ERROR_CODE_OK;
    }
}

// Set value of given attribute.
int CGXDLMSCommon::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else
    {
        SetValue(e.GetValue());
    }

    return DLMS_ERROR_CODE_OK;
}
