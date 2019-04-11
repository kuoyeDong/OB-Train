/**
 * Blockly Demos: Block Factory Blocks
 *
 * Copyright 2012 Google Inc.
 * https://developers.google.com/blockly/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @fileoverview Blocks for Blockly's Block Factory application.
 * @author fraser@google.com (Neil Fraser)
 */

'use strict';





function transHex(num, len) {
    num = num * 1;
    let max = Math.pow(256, len) - 1;
    if (num > max) {
        num = max;
    }
    let goalNum = num.toString(16);
    let goalNumLen = goalNum.length;
    let wantAddZeroSum = len * 2 - goalNumLen;
    let i;
    for ( i = 0; i < wantAddZeroSum; i++) {
        goalNum = "0" + goalNum;
    }
    return goalNum;
}

Blockly.Blocks['OB_DELAYTIME'] = {
    init: function() {
        this.appendValueInput("time")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_DELAYTIME);
        this.appendDummyInput()
        .appendField("s");
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.DOBOT_DELAYTIME_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

Blockly.JavaScript['OB_DELAYTIME'] = function(block) {
    var value_time = Blockly.JavaScript.valueToCode(block, 'time', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"wait'+value_time+'s"\n';
    return code;
};


Blockly.Blocks['OB_Drone_Lock'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_LOCK);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_LOCK_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_Lock'] = function(block) {
    var code = '"ff5580808080080003000000000000000001"\n';
    return code;
};

Blockly.Blocks['OB_Drone_UnLock'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_UNLOCK);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_UNLOCK_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_UnLock'] = function(block) {
    var code = '"ff5580808080080003000000000000000002"\n';
    return code;
};


Blockly.Blocks['OB_Drone_CAL_GYRO'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CAL_GYRO);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_CAL_GYRO_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_CAL_GYRO'] = function(block) {
    var code = '"ff5580808080080001000000000000000001"\n';
    return code;
};

Blockly.Blocks['OB_Drone_CAL_ACC'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CAL_ACC);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_CAL_ACC_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_CAL_ACC'] = function(block) {
    var code = '"ff558080808008000c900000000000000001"\n';
    return code;
};

Blockly.Blocks['OB_Drone_CAL_MAG'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CAL_MAG);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_CAL_MAG_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

Blockly.JavaScript['OB_Drone_CAL_MAG'] = function(block) {
    var code = '"ff5580808080080002000000000000000001"\n';
    return code;
};


Blockly.Blocks['OB_Drone_AUTO_TAKEOFF'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_AUTO_TAKEOFF);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_AUTO_TAKEOFF_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_AUTO_TAKEOFF'] = function(block) {
    var code = '"ff5580808080080004000000000000000001"\n';
    return code;
};

Blockly.Blocks['OB_Drone_AUTO_LANDING'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_AUTO_LANDING);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_AUTO_LANDING_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};
Blockly.JavaScript['OB_Drone_AUTO_LANDING'] = function(block) {
    var code = '"ff5580808080080004000000000000000002"\n';
    return code;
};


Blockly.Blocks['OB_DRONE_ALTITUDE_POSITION_CONTROL'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_ALTITUDE_POSITION_CONTROL);
        this.appendValueInput("Altitude")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_RELA_ALTITUDE_SETTING);
        this.appendValueInput("Pitch")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_RELA_POSITION_PITCH_SETTING);
        this.appendValueInput("Roll")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_RELA_POSITION_ROLL_SETTING);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_ALTITUDE_POSITION_CONTROL_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

Blockly.JavaScript['OB_DRONE_ALTITUDE_POSITION_CONTROL'] = function(block) {
    var value_Altitude = Blockly.JavaScript.valueToCode(block, 'Altitude', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    var value_Pitch = Blockly.JavaScript.valueToCode(block, 'Pitch', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    var value_Roll = Blockly.JavaScript.valueToCode(block, 'Roll', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    var code = '"ff558080808008002C'+transHex(value_Altitude,1)+''+transHex(value_Pitch,1)+''+transHex(value_Roll,1)+'000002"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_RISE'] = {
    init: function() {
        this.appendValueInput("fly_rise")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_RISE);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_RISE_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_RISE'] = function(block) {
    var value_fly_rise = Blockly.JavaScript.valueToCode(block, 'fly_rise', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C'+transHex(value_fly_rise,2)+'00000000000001"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_DOWN'] = {
    init: function() {
        this.appendValueInput("fly_down")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_DOWN);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_DOWN_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_DOWN'] = function(block) {
    var value_fly_down = Blockly.JavaScript.valueToCode(block, 'fly_down', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C'+transHex(value_fly_down,2)+'00000000000002"\n';
    return code;
};




Blockly.Blocks['OB_DRONE_FLY_ONWARD'] = {
    init: function() {
        this.appendValueInput("fly_onward")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_ONWARD);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_ONWARD_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_ONWARD'] = function(block) {
    var value_fly_onward = Blockly.JavaScript.valueToCode(block, 'fly_onward', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C0000'+transHex(value_fly_onward,2)+'0000000001"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_BACK'] = {
    init: function() {
        this.appendValueInput("fly_back")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_BACK);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_BACK_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_BACK'] = function(block) {
    var value_fly_back = Blockly.JavaScript.valueToCode(block, 'fly_back', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C0000'+transHex(value_fly_back,2)+'0000000002"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_LEFT'] = {
    init: function() {
        this.appendValueInput("fly_left")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_LEFT);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_LEFT_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_LEFT'] = function(block) {
    var value_fly_left = Blockly.JavaScript.valueToCode(block, 'fly_left', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C00000000'+transHex(value_fly_left,2)+'000001"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_RIGHT'] = {
    init: function() {
        this.appendValueInput("fly_right")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_RIGHT);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_CM);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_RIGHT_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_RIGHT'] = function(block) {
    var value_fly_right = Blockly.JavaScript.valueToCode(block, 'fly_right', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C00000000'+transHex(value_fly_right,2)+'000002"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_YAW_LEFT'] = {
    init: function() {
        this.appendValueInput("fly_yaw_left")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_YAW_LEFT);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_ANGLEUNIT);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_YAW_LEFT_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_YAW_LEFT'] = function(block) {
    var value_fly_yaw_left = Blockly.JavaScript.valueToCode(block, 'fly_yaw_left', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C000000000000'+transHex(value_fly_yaw_left,2)+'01"\n';
    return code;
};

Blockly.Blocks['OB_DRONE_FLY_YAW_RIGHT'] = {
    init: function() {
        this.appendValueInput("fly_yaw_right")
        .setCheck("Number")
        .appendField(Blockly.Msg.OB_DRONE_FLY_YAW_RIGHT);
        this.appendDummyInput()
        .appendField(Blockly.Msg.OB_DRONE_ANGLEUNIT);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(100);
        this.setTooltip(Blockly.Msg.OB_DRONE_FLY_YAW_RIGHT_TOOLTIP);
        this.setHelpUrl('Http://www.on-bright.com');
    }
};

//控制两个字节
Blockly.JavaScript['OB_DRONE_FLY_YAW_RIGHT'] = function(block) {
    var value_fly_yaw_right = Blockly.JavaScript.valueToCode(block, 'fly_yaw_right', Blockly.JavaScript.ORDER_ATOMIC) || 0.5;
    var code = '"ff558080808008002C000000000000'+transHex(value_fly_yaw_right,2)+'02"\n';
    return code;
};


Blockly.Blocks['dobot_home'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_BASEIC_HOME);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_BASEIC_HOME_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_home'] = function(block) {
    var code = 'dType.SetHOMECmdEx(api, 0, 1)\n';
    return code;
};

Blockly.Blocks['dobot_time'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_BASEIC_TIME);
        this.setInputsInline(true);
        this.setOutput(true, "Number");
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_BASEIC_TIME_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_time'] = function(block) {
    var code = 'dType.gettime()[0]';
    return [code, Blockly.JavaScript.ORDER_NONE];
};

Blockly.Blocks['dobot_selectendtype'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_BASEIC_SETENDTYPE)
        .appendField(new Blockly.FieldDropdown(
                         [[Blockly.Msg.DOBOT_BASEIC_SETENDTYPE_SUCTIONCAP, "59.7"],
                          [Blockly.Msg.DOBOT_BASEIC_SETENDTYPE_GRIPPER, "59.701"],
                          [Blockly.Msg.DOBOT_BASEIC_SETENDTYPE_LASER, "70"],
                          [Blockly.Msg.DOBOT_BASEIC_SETENDTYPE_PEN, "61"]]), "type");
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_BASEIC_SETENDTYPE_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_selectendtype'] = function(block) {
    var dropdown_type = block.getFieldValue('type');
    if(dropdown_type == "59.701")
        dropdown_type = "59.7";
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetEndEffectorParamsEx(api, '+dropdown_type+', 0, 0, 1)\n';
    return code;
};

Blockly.Blocks['dobot_SetEndEffectorParams'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_BASEIC_SETENDEFFECTORPARAMS);
        this.appendValueInput("xBias")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_xBias);
        this.appendValueInput("yBias")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_yBias);
        this.appendValueInput("zBias")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_zBias);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_BASEIC_SETENDEFFECTORPARAMS_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_SetEndEffectorParams'] = function(block) {
    var value_xbias = Blockly.JavaScript.valueToCode(block, 'xBias', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    var value_ybias = Blockly.JavaScript.valueToCode(block, 'yBias', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    var value_zbias = Blockly.JavaScript.valueToCode(block, 'zBias', Blockly.JavaScript.ORDER_ATOMIC) || 0.0;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetEndEffectorParamsEx(api, '+value_xbias+',  '+value_ybias+',  '+value_zbias+', 1)\n';
    return code;
};

Blockly.Blocks['dobot_setptpjointparams'] = {
    init: function() {
        this.appendValueInput("Velocity")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_SET_JOINT_SPEED)
        .appendField(Blockly.Msg.DOBOT_VELOCITY);
        this.appendValueInput("Acceleration")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_ACCELERATION);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_SET_JOINT_SPEED_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_setptpjointparams'] = function(block) {
    var vel = Blockly.JavaScript.valueToCode(block, 'Velocity', Blockly.JavaScript.ORDER_ATOMIC) || 20;
    var acc = Blockly.JavaScript.valueToCode(block, 'Acceleration', Blockly.JavaScript.ORDER_ATOMIC) || 50;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetPTPJointParamsEx(api,'+vel+','+acc+','
            +vel+','+acc+','
            +vel+','+acc+','
            +vel+','+acc+',1)\n';
    return code;
};

Blockly.Blocks['dobot_setptpoordinateparams'] = {
    init: function() {
        this.appendValueInput("Velocity")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_SET_COORDINATE_SPEED)
        .appendField(Blockly.Msg.DOBOT_VELOCITY);
        this.appendValueInput("Acceleration")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_ACCELERATION);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_SET_COORDINATE_SPEED_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_setptpoordinateparams'] = function(block) {
    var vel = Blockly.JavaScript.valueToCode(block, 'Velocity', Blockly.JavaScript.ORDER_ATOMIC) || 20;
    var acc = Blockly.JavaScript.valueToCode(block, 'Acceleration', Blockly.JavaScript.ORDER_ATOMIC) || 50;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetPTPCoordinateParamsEx(api,'+vel+','+acc+','
            +vel+','+acc+',1)\n';
    return code;
};

Blockly.Blocks['dobot_setptplparams'] = {
    init: function() {
        this.appendValueInput("lVelocity")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_SET_L_SPEED)
        .appendField(Blockly.Msg.DOBOT_VELOCITY);
        this.appendValueInput("lAcceleration")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_ACCELERATION);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_SET_JOINT_L_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};
Blockly.JavaScript['dobot_setptplparams'] = function(block) {
    var value_lvelocity = Blockly.JavaScript.valueToCode(block, 'lVelocity', Blockly.JavaScript.ORDER_ATOMIC) || 20;
    var value_lacceleration = Blockly.JavaScript.valueToCode(block, 'lAcceleration', Blockly.JavaScript.ORDER_ATOMIC) || 50;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetPTPLParamsEx(api,'+value_lvelocity+','+value_lacceleration+',1)\n';
    return code;
};

Blockly.Blocks['dobot_setlinearspeed'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_SET_LINEAR_SPEED);
        this.appendValueInput("velocity")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_VELOCITY_RATIO);
        this.appendValueInput("acceleration")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_ACCELERATION_RATIO);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip('');
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_setlinearspeed'] = function(block) {
    var value_velocity = Blockly.JavaScript.valueToCode(block, 'velocity', Blockly.JavaScript.ORDER_ATOMIC) || 20;
    var value_acceleration = Blockly.JavaScript.valueToCode(block, 'acceleration', Blockly.JavaScript.ORDER_ATOMIC) || 50;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetPTPCommonParamsEx(api,'+value_velocity+','+value_acceleration+',1)\n';
    return code;
};

Blockly.Blocks['dobot_setjumpheight'] = {
    init: function() {
        this.appendDummyInput()
        .appendField(Blockly.Msg.DOBOT_SET_JUMP_HEIGHT);
        this.appendValueInput("height")
        .setCheck("Number")
        .appendField(Blockly.Msg.DOBOT_height);
        this.setInputsInline(true);
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(330);
        this.setTooltip(Blockly.Msg.DOBOT_SET_JUMP_HEIGHT_TOOLTIP);
        this.setHelpUrl('http://www.dobot.cc/');
    }
};

Blockly.JavaScript['dobot_setjumpheight'] = function(block) {
    var value_height = Blockly.JavaScript.valueToCode(block, 'height', Blockly.JavaScript.ORDER_ATOMIC) || 20;
    // TODO: Assemble JavaScript into code variable.
    var code = 'dType.SetPTPJumpParamsEx(api,'+value_height+',100,1)\n';
    return code;
};
