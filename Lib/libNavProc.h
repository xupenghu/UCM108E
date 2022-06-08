#ifndef	__LIB_NAV_PROC_H_
#define	__LIB_NAV_PROC_H_

#ifndef	__LIB_NAV_PROC_C_
#define	EXT_NAVPROC		extern
#else
#define	EXT_NAVPROC
#endif

typedef enum
{
	PVT_STATE_NULL = 0,	// fix failed
	PVT_STATE_OK = 1	// fix successed
}ENUM_PVT_STATE;


typedef struct
{
	S16 sYear;
	U08 ucMon;
	U08 ucDay;
	U08 ucHour;
	U08 ucMin;
	F32 fSec;
}STU_RTC, *PSTU_RTC;

typedef struct
{
	F32 fDat;
	F32 fErr;
}TYP_F32, *PTYP_F32;

typedef struct
{
	TYP_F32 tX;			// x[unit:m or m/s]
	TYP_F32 tY;			// y[unit:m or m/s]
	TYP_F32 tZ;			// z[unit:m or m/s]
	TYP_F32 tT;			// t[unit:s or s/s]
}STU_COOR_XYZ, *PSTU_COOR_XYZ;

typedef struct
{
	TYP_F32 tLon;		// longitude[unit:rad]
	TYP_F32 tLat;		// latitude[unit:rad]
	F32 fAlt;			// altitude[unit:m]
}STU_COOR_LLA, *PSTU_COOR_LLA;

typedef struct
{
	F32 fEst;			// eastward speed[unit:m/s]
	F32 fNrt;			// northward speed[unit:m/s]
	F32 fUp;			// upward speed[unit:m/s]
	F32 fV;				// speed over ground[unit:m/s]
	F32 fHeading;		// course over ground[unit:rad]
}STU_COOR_ENU, *PSTU_COOR_ENU;

typedef struct
{
	STU_COOR_XYZ tPos;	// position
	STU_COOR_XYZ tVel;	// velocity
}STU_PVT_XYZ, *PSTU_PVT_XYZ;

typedef struct
{
	ENUM_PVT_STATE eState;	// fix status
	STU_PVT_XYZ tXyz;
	STU_COOR_LLA tLla;
	STU_COOR_ENU tEnu;
	F32 fPdop;
	F32 fHdop;
	F32 fVdop;
	F32 fTdop;
	F32 fGdop;
}STU_USR_PVT, *PSTU_USR_PVT;

typedef void (*CALL_BACK)(IN const PSTU_RTC pRtc, IN const PSTU_USR_PVT pPvt);
EXT_NAVPROC void GnssStart(IN const CALL_BACK GetPvt, IN U08 ucLnaEnablePin, IN BOOL bFlashOpt, IN const U08 *pAgnssData);
EXT_NAVPROC void GnssStop();
EXT_NAVPROC void TicIsr();

#endif
