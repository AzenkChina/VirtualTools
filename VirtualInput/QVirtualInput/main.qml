import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: root
    visible: true
    width: 560
    height: 360
    minimumWidth: 560
    minimumHeight: 360
    maximumWidth: 560
    maximumHeight: 360
	opacity: 0.95
    title: "VirtualInput"

    Button {
        id: key_1
        x: 20
        y: 20
        text: qsTr("1")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_1_clicked()
        }
    }

    Button {
        id: key_2
        x: 160
        y: 20
        text: qsTr("2")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_2_clicked()
        }
    }

    Button {
        id: key_3
        x: 300
        y: 20
        text: qsTr("3")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_3_clicked()
        }
    }

    Button {
        id: key_4
        x: 20
        y: 80
        text: qsTr("4")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_4_clicked()
        }
    }

    Button {
        id: key_5
        x: 160
        y: 80
        text: qsTr("5")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_5_clicked()
        }
    }

    Button {
        id: key_6
        x: 300
        y: 80
        text: qsTr("6")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_6_clicked()
        }
    }

    Button {
        id: key_7
        x: 20
        y: 140
        text: qsTr("7")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_7_clicked()
        }
    }

    Button {
        id: key_8
        x: 160
        y: 140
        text: qsTr("8")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_8_clicked()
        }
    }

    Button {
        id: key_9
        x: 300
        y: 140
        text: qsTr("9")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_9_clicked()
        }
    }

    Button {
        id: key_0
        x: 20
        y: 200
        text: qsTr("0")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_0_clicked()
        }
    }

    Button {
        id: key_enter
        x: 160
        y: 200
        text: qsTr("删除")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_enter_clicked()
        }
    }

    Button {
        id: key_del
        x: 300
        y: 200
        text: qsTr("回车")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_del_clicked()
        }
    }

    Button {
        id: key_prog
        x: 440
        y: 200
        text: qsTr("编程")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_prog_clicked()
        }
    }

    Button {
        id: key_up
        x: 440
        y: 20
        height: 60
        text: qsTr("上翻")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_up_pressed()
        }
        onReleased: {
            input.on_key_up_released()
        }
    }

    Button {
        id: key_down
        x: 440
        y: 120
        height: 60
        text: qsTr("下翻")
        font.pointSize: 12
        font.family: "Tahoma"
        onClicked: {
            input.on_key_down_pressed()
        }
        onReleased: {
            input.on_key_down_released()
        }
    }

    CheckBox {
        id:check_bat_rtc
        x: 20
        y: 254
        text: qsTr("时钟电池欠压")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_bat_rtc.checked == true){
                input.on_check_bat_rtc_stateChanged(1)
            }
            else{
                input.on_check_bat_rtc_stateChanged(0)
            }
        }
    }

    CheckBox {
        id:check_bat_backup
        x: 20
        y: 300
        text: qsTr("抄表电池欠压")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_bat_backup.checked == true){
                input.on_check_bat_backup_stateChanged(1)
            }
            else{
                input.on_check_bat_backup_stateChanged(0)
            }
        }
    }

    CheckBox {
        id: check_main_cover
        x: 160
        y: 254
        text: qsTr("表盖打开")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_main_cover.checked == true){
                input.on_check_main_cover_stateChanged(1)
            }
            else{
                input.on_check_main_cover_stateChanged(0)
            }
        }
    }

    CheckBox {
        id: check_sub_cover
        x: 160
        y: 300
        text: qsTr("端盖打开")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_sub_cover.checked == true){
                input.on_check_sub_cover_stateChanged(1)
            }
            else{
                input.on_check_sub_cover_stateChanged(0)
            }
        }
    }

    CheckBox {
        id: check_magnetic
        x: 300
        y: 254
        text: qsTr("电磁场干扰")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_magnetic.checked == true){
                input.on_check_magnetic_stateChanged(1)
            }
            else{
                input.on_check_magnetic_stateChanged(0)
            }
        }
    }

    CheckBox {
        id: check_relay
        x: 300
        y: 300
        text: qsTr("继电器损坏")
        font.family: "Tahoma"
        checked: false
        onCheckedChanged:{
            if(check_relay.checked == true){
                input.on_check_relay_stateChanged(1)
            }
            else{
                input.on_check_relay_stateChanged(0)
            }
        }
    }
}
