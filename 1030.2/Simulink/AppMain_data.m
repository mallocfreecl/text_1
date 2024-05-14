clear all

%% Create enumerated types
if exist('eMotorFaultType','class')~=8
    Simulink.defineIntEnumType('eMotorFaultType',...
        {'eMotorFaultTypeNormal','eMotorFaultShortCircuit','eMotorFaultTypeOpenLoad','eMotorFaultTypeLock'},...
        [0,1,2,3]);
end

if exist('EnumMotorFaultType','class')~=8
    Simulink.defineIntEnumType('EnumMotorFaultType',...
        {'Short','OverCurrent','OpenLoad','Others'},...
        [0,1,2,4]);
end

if exist('EnumMotorFaultCode','class')~=8
    Simulink.defineIntEnumType('EnumMotorFaultCode',...
        {'FaultNormal','FaultShort','FaultOverCurrent','FaultOpenLoad','FaultOthers'},...
        [0,1,2,4,8]);
end

if exist('EnumFaultRecoverState','class')~=8
    Simulink.defineIntEnumType('EnumFaultRecoverState',...
        {'CalcTHR','Wait','Recover'},...
        [0,1,2]);
end

if exist('EnumParamStatus','class')~=8
    Simulink.defineIntEnumType('EnumParamStatus',...
        {'Normal','Low','High','LowEE','HighEE'},...
        [0,1,2,4,8]);
end

%%
if exist('ScalarStatus_t','class')~=8
    Simulink.defineIntEnumType('ScalarStatus_t',...
        {'eNormal','ePreLow','eLow','ePreHigh','eHigh'},...
        [0,1,2,4,8]);
end

%% Create constants which will not change during simulation or code generation
TaskTimeBase = struct;

TaskTimeBase.PwmIn = 10e-3;
TaskTimeBase.SpeedCtrl= 10e-3;
TaskTimeBase.FaultDiag = 100e-3;
TaskTimeBase.SleepMode = 100e-3;
TaskTimeBase.FeedBack = 100e-3;

CtrlConst = struct;

% PwmIn
CtrlConst.PwmIn = struct;
CtrlConst.PwmIn.BusTimeOutCntTHR = uint16(200); %Bus idle State time out time,unit:ms

% Speed Ctrl by pwm in
CtrlConst.SpeedCtrl.PwmIn = struct;
CtrlConst.SpeedCtrl.PwmIn.PowerOnBlankTime = uint16(200e-3/TaskTimeBase.SpeedCtrl);

CtrlConst.SpeedCtrl.PwmIn.Freq = struct;
CtrlConst.SpeedCtrl.PwmIn.Freq.Min = uint16(970);%0.1Hz
CtrlConst.SpeedCtrl.PwmIn.Freq.Max = uint16(3090);%0.1Hz
CtrlConst.SpeedCtrl.PwmIn.Freq.Tolerance = uint16(3);%0.1Hz
CtrlConst.SpeedCtrl.PwmIn.Freq.ValidJudgeFilterTime = uint16(200e-3/TaskTimeBase.SpeedCtrl);

CtrlConst.SpeedCtrl.PwmIn.Duty = struct;
CtrlConst.SpeedCtrl.PwmIn.Duty.Default = uint16(80);
CtrlConst.SpeedCtrl.PwmIn.Duty.Tolerance = uint16(10); % 10*0.1%=1%

CtrlConst.SpeedCtrl.DefaultSector = uint16(1);

CtrlConst.SpeedCtrl.PwmIn.Duty.Filter = struct;
CtrlConst.SpeedCtrl.PwmIn.Duty.Filter.BufLen = uint8(6);
CtrlConst.SpeedCtrl.PwmIn.Duty.Filter.DutyTolerance=uint16(20); % 20*0.1%=2%
CtrlConst.SpeedCtrl.PwmIn.Duty.Filter.DutySameCntTHR = uint16(4);

CtrlConst.SpeedCtrl.PwmIn.TargetSpeed = struct;
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.Min = int16(750);%rpm
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.Max = int16(2530);%rpm
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.Full = int16(2530);%rpm
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.PwmBusShortGND = int16(0);%rpm
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.PwmBusShortBat = int16(0);%rpm
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.FreqInvalid = int16(CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.Full);%rpm

CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.DelayOutTime = struct;
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.DelayOutTime.PwmBusShortGND = uint16((4-double(CtrlConst.PwmIn.BusTimeOutCntTHR)*0.001)/TaskTimeBase.SpeedCtrl);%
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.DelayOutTime.PwmBusShortBat = uint16((2-double(CtrlConst.PwmIn.BusTimeOutCntTHR)*0.001)/TaskTimeBase.SpeedCtrl);%
CtrlConst.SpeedCtrl.PwmIn.TargetSpeed.DelayOutTime.FreqInvalid = uint16(4/TaskTimeBase.SpeedCtrl);%


%Fault Diag
CtrlConst.FaultDiag.Voltage = struct;
CtrlConst.FaultDiag.Voltage.FilterTime = uint16(1/TaskTimeBase.FaultDiag);

CtrlConst.FaultDiag.Voltage.THR = struct;
CtrlConst.FaultDiag.Voltage.THR.LowEntry = int16(75); % 0.1V
CtrlConst.FaultDiag.Voltage.THR.LowExit = int16(82); % 0.1V
CtrlConst.FaultDiag.Voltage.THR.HighEntry = int16(175); % 0.1V
CtrlConst.FaultDiag.Voltage.THR.HighExit = int16(170); % 0.1V

CtrlConst.FaultDiag.AmbTemperature = struct;
CtrlConst.FaultDiag.AmbTemperature.FilterTime = uint16(1/TaskTimeBase.FaultDiag);
CtrlConst.FaultDiag.AmbTemperature.PreOverFilterTime = uint16(1/TaskTimeBase.SpeedCtrl);

CtrlConst.FaultDiag.AmbTemperature.THR = struct;
CtrlConst.FaultDiag.AmbTemperature.THR.LowEntry = int16(-40); % degree centigrade
CtrlConst.FaultDiag.AmbTemperature.THR.LowExit = int16(-38); % degree centigrade
CtrlConst.FaultDiag.AmbTemperature.THR.PreOverEntry = int16(135); % degree centigrade
CtrlConst.FaultDiag.AmbTemperature.THR.PreOverExit = int16(130); % degree centigrade
CtrlConst.FaultDiag.AmbTemperature.THR.OverEntry = int16(140); % degree centigrade
CtrlConst.FaultDiag.AmbTemperature.THR.OverExit = int16(135); % degree centigrade

%Sleep mode
CtrlConst.SpeedMode = struct;
CtrlConst.SpeedMode.UseIG=false;
CtrlConst.SpeedMode.UsePwmIn=true;
CtrlConst.SpeedMode.ValidCntTHR = uint16((2-double(CtrlConst.PwmIn.BusTimeOutCntTHR)*0.001)/TaskTimeBase.SleepMode);% 2s
CtrlConst.SpeedMode.WaitTimeTHR = uint16((2)/TaskTimeBase.SleepMode);% 2s

%FeedBack
CtrlConst.FeedBack.PowerUpBlankTime = uint16(3.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.MotorShort = uint16(60/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.MotorOpenLoad = uint16(90/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.MotorLock = uint16(160/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.MotorDryRunning = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.Voltage = uint16(30/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.AmbT = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.StartDelay.AmbTPreOver = uint16(0/TaskTimeBase.FeedBack);

CtrlConst.FeedBack.EndDelay.MotorShort = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.MotorOpenLoad = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.MotorLock = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.MotorDryRunning = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.Voltage = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.AmbT = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.EndDelay.AmbTPreOver = uint16(0/TaskTimeBase.FeedBack);

CtrlConst.FeedBack.LowTime.MotorShort = uint16(3.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.MotorOpenLoad = uint16(3.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.MotorLock = uint16(2.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.MotorDryRunning = uint16(0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.Voltage = uint16(2.0/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.AmbT = uint16(3/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.AmbTPreOver = uint16(1.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.LowTime.Normal = uint16(0.5/TaskTimeBase.FeedBack);%0.5
CtrlConst.FeedBack.LowTime.PwmInError = uint16(0/TaskTimeBase.FeedBack);

CtrlConst.FeedBack.HighTime.MotorShort = uint16(6.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.MotorOpenLoad = uint16(6.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.MotorLock = uint16(7.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.MotorDryRunning = uint16(1/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.Voltage = uint16(8/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.AmbT = uint16(7/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.AmbTPreOver = uint16(8.5/TaskTimeBase.FeedBack);
CtrlConst.FeedBack.HighTime.Normal = uint16(9.5/TaskTimeBase.FeedBack);%9.5
CtrlConst.FeedBack.HighTime.PwmInError = uint16(1/TaskTimeBase.FeedBack);

CtrlConst.FeedBack.LowTime=[CtrlConst.FeedBack.LowTime.MotorShort,CtrlConst.FeedBack.LowTime.MotorOpenLoad,CtrlConst.FeedBack.LowTime.AmbT,CtrlConst.FeedBack.LowTime.MotorLock,CtrlConst.FeedBack.LowTime.Voltage,CtrlConst.FeedBack.LowTime.AmbTPreOver,CtrlConst.FeedBack.LowTime.Normal];
CtrlConst.FeedBack.HighTime=[CtrlConst.FeedBack.HighTime.MotorShort,CtrlConst.FeedBack.HighTime.MotorOpenLoad,CtrlConst.FeedBack.HighTime.AmbT,CtrlConst.FeedBack.HighTime.MotorLock,CtrlConst.FeedBack.HighTime.Voltage,CtrlConst.FeedBack.HighTime.AmbTPreOver,CtrlConst.FeedBack.HighTime.Normal];
%[CtrlConst.FeedBack.HighTime.AmbTPreOver,CtrlConst.FeedBack.HighTime.Normal]
%[CtrlConst.FeedBack.LowTime.Voltage,CtrlConst.FeedBack.LowTime.AmbT,CtrlConst.FeedBack.LowTime.MotorShort,CtrlConst.FeedBack.LowTime.MotorOpenLoad,CtrlConst.FeedBack.LowTime.MotorLock,CtrlConst.FeedBack.LowTime.MotorDryRunning,CtrlConst.FeedBack.LowTime.Normal]
%[CtrlConst.FeedBack.HighTime.Voltage,CtrlConst.FeedBack.HighTime.AmbT,CtrlConst.FeedBack.HighTime.MotorShort,CtrlConst.FeedBack.HighTime.MotorOpenLoad,CtrlConst.FeedBack.HighTime.MotorLock,CtrlConst.FeedBack.HighTime.MotorDryRunning,CtrlConst.FeedBack.HighTime.Normal]

%% Create tunable controller parameter structure for code generation
ctrlParams = struct;

%% Create struct without init data

%% PwmInStatus
PwmInStatus_t = struct;
PwmInStatus_t.ShortGND = false;
PwmInStatus_t.ShortBAT = false;
PwmInStatus_t.Freq = uint16(0);
PwmInStatus_t.HighDuty = uint16(0);

tmp_BusName = Simulink.Bus.createObject(PwmInStatus_t);
tmp_Bus = evalin('base',tmp_BusName.busName);
evalin('base',['clear ' tmp_BusName.busName]);
assignin('base','PwmInStatus_t',tmp_Bus);
clear tmp_BusName tmp_Bus

%%
MotorFaultCnt_t = struct;
MotorFaultCnt_t.Short = uint8(0);
MotorFaultCnt_t.Open = uint8(0);
MotorFaultCnt_t.Lock = uint8(0);

tmp_BusName = Simulink.Bus.createObject(MotorFaultCnt_t);
tmp_Bus = evalin('base',tmp_BusName.busName);
evalin('base',['clear ' tmp_BusName.busName]);
assignin('base','MotorFaultCnt_t',tmp_Bus);
clear tmp_BusName tmp_Bus

%% Create parameter structure in generated code
tmp_P = Simulink.Parameter;
tmp_P.Value=ctrlParams;
tmp_P.CoderInfo.StorageClass='ExportedGlobal';
ctrlParams = tmp_P;
clear tmp_P

tmp_BusInfo = Simulink.Bus.createObject(ctrlParams.Value);
stCtrlParams= eval(tmp_BusInfo.busName);
ctrlParams.DataType='Bus: stCtrlParams';
clear(tmp_BusInfo.busName);
clear tmp_BusInfo;

%%
%

