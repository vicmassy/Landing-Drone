
"use strict";

let RawMagnetic = require('./RawMagnetic.js');
let MotorPWM = require('./MotorPWM.js');
let Supply = require('./Supply.js');
let PositionXYCommand = require('./PositionXYCommand.js');
let HeightCommand = require('./HeightCommand.js');
let VelocityZCommand = require('./VelocityZCommand.js');
let MotorStatus = require('./MotorStatus.js');
let VelocityXYCommand = require('./VelocityXYCommand.js');
let YawrateCommand = require('./YawrateCommand.js');
let Altimeter = require('./Altimeter.js');
let HeadingCommand = require('./HeadingCommand.js');
let AttitudeCommand = require('./AttitudeCommand.js');
let Compass = require('./Compass.js');
let ControllerState = require('./ControllerState.js');
let MotorCommand = require('./MotorCommand.js');
let Altitude = require('./Altitude.js');
let RawRC = require('./RawRC.js');
let RuddersCommand = require('./RuddersCommand.js');
let ServoCommand = require('./ServoCommand.js');
let RawImu = require('./RawImu.js');
let RC = require('./RC.js');
let ThrustCommand = require('./ThrustCommand.js');

module.exports = {
  RawMagnetic: RawMagnetic,
  MotorPWM: MotorPWM,
  Supply: Supply,
  PositionXYCommand: PositionXYCommand,
  HeightCommand: HeightCommand,
  VelocityZCommand: VelocityZCommand,
  MotorStatus: MotorStatus,
  VelocityXYCommand: VelocityXYCommand,
  YawrateCommand: YawrateCommand,
  Altimeter: Altimeter,
  HeadingCommand: HeadingCommand,
  AttitudeCommand: AttitudeCommand,
  Compass: Compass,
  ControllerState: ControllerState,
  MotorCommand: MotorCommand,
  Altitude: Altitude,
  RawRC: RawRC,
  RuddersCommand: RuddersCommand,
  ServoCommand: ServoCommand,
  RawImu: RawImu,
  RC: RC,
  ThrustCommand: ThrustCommand,
};
