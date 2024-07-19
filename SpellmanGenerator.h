#include <iostream>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #error "Linux Kernel Not Yet Supported"
#elif defined(__gnu_linux__)
    #error "GNU Linux Not Yet Supported"
#else 
    #error "Platform Not Supported"
#endif

typedef enum
{
    ProgramkV = 10,
    ProgrammA,
    ProgramFilamentLimit,
    ProgramFilamentPreheatLimit,
    RequestkVSetPoint,
    RequestmASetPoint,
    RequestFilamentLimitSetPoint,
    RequestFilamentPreheatSetPoint,
    RequestAnalogReadBacks = 19,
    RequestHVOnHours = 21,
    RequestStatus,
    RequestSoftwareVersion,
    RequestModelNumber = 26,
    RequestUserConfig,
    RequestScaling,
    ResetHVOnHours = 30,
    ResetFaluts,
    SetFilamentSize,
    RequestPowerLimits = 38,
    RequestFPGARev = 43,
    RequestkVMonitor = 60,
    RequestNeg15LVPS = 65,
    RequestFaults = 68,
    RequestSystemVoltages,
    FilamentControl,
    XRVControllerPresent,
    ProgramPowerLimits = 97,
    ProgramHVOn,
    LocalRemoteModeSet
} spellmanCommands_e;

/*!
 *  @brief Data Structure to store all current fault signals for the generator
 *  Total Size = 27 bytes
 */
typedef struct
{
    bool FilamentSelect;
    bool OverTemp;
    bool OverVolt;
    bool UnderVolt;
    bool OverCurrent;
    bool UnderCurrent;
    bool OverTempAnnode;
    bool OverTempCathode;
    bool InverterAnnode;
    bool InverterCathode;
    bool FilamentFeedback;
    bool AnnodeArc;
    bool CathodeArc;
    bool CableConnectAnnode;
    bool CConnectCahtode;
    bool ACLineMonitorAnnode;
    bool ACLineMonitorCathode;
    bool DCRailAnnode;
    bool DCRailCathode;
    bool LVPSN15;
    bool LVPSP15;
    bool WatchDog;
    bool BoardOverTemp;
    bool OverPower;
    bool KVDiff;
    bool mADiff;
    bool InverterNotReady;
} GeneratorFaults_t;

/*!
 *  @brief Data Structure to store all current status signals for the generator
 *  Total Size = 17 bytes
 */
typedef struct
{
    bool HVEnabled;       //1 = enabled
    bool Interlock1;      //1 = closed
    bool Interlock2;      //1 = closed
    bool ECRMode;         //1 = active
    bool PSFault;         //1 = falut
    bool RemoteMode;      //0 = remote; 1 = local
    bool FilamentEnabled; //1 = enabled
    bool FilamentType;    //1 = Large
    bool PreWarn;         //1 = xrays eminent
    bool largeFilConf;
    bool smallFilConf;
    bool R1;
    bool R2;
    bool R3;
    bool R4;
    bool PSReady;         //1 = ready
    bool InternalInterlock; //1 = closed
} GeneratorStatus_t;

/*!
 *  @brief Data Structure used to store tube metrics in a function returnable fashion that is low on size
 *  Total Size = 12 bytes
 */
typedef struct
{
    float kV;
    float mA;
    float filCurrent;
} TubeData_t;

/*!
 *  @brief Data Structure used to store all pertinent data for our tube
 *  Total Size = 56 bytes
 */
typedef struct
{
    //Acutal Data
    TubeData_t tubeData;
    //Conversions
    float conv_mA_SP;
    float conv_mA_MSR;
    float conv_kV_SP;
    float conv_kV_MSR;
    float conv_Fil_SP;
    float conv_Fil_MSR;
    GeneratorStatus_t status;
    GeneratorFaults_t faults;
    /*might want to add monitors for peripheral signals as well*/
} GeneratorData_t;

/*!
 *  @brief Our main interface class for ALL Generator Types
 */
class SpellmanGenerator
{
    private:
    GeneratorData_t tube_meas;
    virtual int writeCommand(char* command);
    virtual int readResponse();
    char ModelNumber_l[12];
    char ModelNumber_s[5];

    public:
    /*Omission of constructor as this is just used as an interface*/
    GeneratorData_t getTubeMeasures();
    GeneratorFaults_t getGeneratorStatus();
    GeneratorStatus_t getGeneraotrStatus();
    virtual int init(){return -1;};
    virtual int setkV(int val){return -1;};
    virtual int setkV(float val){return -1;};
    virtual int setmA(int val){return -1;};
    virtual int setmA(float val){return -1;};
    virtual int resetGenFaults(){return -1;};
    virtual int setFilamentLimit(float val){return -1;};
    virtual int setFilamentLimit(int val){return -1;};
    virtual int setFilamentPreheatLimit(float val){return -1;};
    virtual int setFilamentPreheatLimit(int val){return -1;};
    virtual int enableHV(bool on = false){return -1;};
    virtual int enableFilament(bool on = false){return -1;};
    virtual int requestAnalogReadBacks(){return -1;};
    virtual int requestFaults(){return -1;};
    virtual int requestStatus(){return -1;};
    virtual int requestGenScaling(){return -1;};
    virtual int requestGeneratorModel(){return -1;};
    virtual int resetGenFaults(){return -1;};
};


/*!
 *  @brief Class for our Spellman Generator using serial connections to the genertaort
 */
class SpellmanGenerator_RS232 : public SpellmanGenerator
{
    public:
    SpellmanGenerator_RS232();
    int init();
    int setkV(int val);
    int setkV(float val);
    int setmA(int val);
    int setmA(float val);
    int resetGenFaults();
    int setFilamentLimit(float val);
    int setFilamentLimit(int val);
    int setFilamentPreheatLimit(float val);
    int setFilamentPreheatLimit(int val);
    int enableHV(bool on = false);
    int enableFilament(bool on = false);
    int requestAnalogReadBacks();
    int requestFaults();
    int requestStatus();
    int requestGenScaling();
    int requestGeneratorModel();
    int resetGenFaults();
    private:
    int calculateCheckSum(char* buf, size_t len);
    int writeCommand(char* command);
    int readResponse();
};

class SpellmanGenerator_Ethernet : public SpellmanGenerator
{
    public:
    SpellmanGenerator_Ethernet();

    private:

};
