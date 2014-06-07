

#ifndef   __HID_ESENSOR_H__
#define   __HID_ESENSOR_H__

//#include <sys/types.h>
#include <sys/timeb.h>
/*
typedef struct _HIDS_DEVICE
{

	// HIDD_ATTRIBUTES
	USHORT  VendorID;
    USHORT  ProductID;
  
	USHORT  SensorID;
	unsigned char   SensorName[100];
	
	USHORT  VersionNumber;

	// HIDP_CAPS
    USHORT   UsagePage;
    UCHAR    ReportID;

	USHORT   BitSize;        // How many bits are devoted to this value?
    LONG     LogicalMin,       LogicalMax;
	ULONG	 Value;
    BOOLEAN  IsAbsolute;

	BOOLEAN  IsRange;
    USHORT   UsageMin,         UsageMax; // If usagemax = Null then UsageMin = Usage

} HIDS_DEVICE, *PHIDX_DEVICE;
*/
typedef struct _HIDS_ENDOSENSOR
{
	// estos seis parámetros identifican un sensor del resto, tambien sirven para diferenciar sensores digitales
	// codificados como botones de sensores analógicos e incluso de sensores digitales de salida  codificados como LEDs
	// Aunque se diferencian las señales Joy y Pain, puede utilizarse un mismo sensor para extraer cada señal
	// en ese caso JoyUsage == PainUsage

	USHORT  VendorID;
    USHORT  ProductID;
    USHORT  SensorID;
	UCHAR   SensorName[100];
	USHORT  VersionNumber;
    
	USHORT  UsagePage;
    USHORT  JoyUsage;		// If JoyUsageMax <> Null then JoyUsage = JoyUsageMin
	USHORT  JoyUsageMax; 
    USHORT  PainUsage;		// If PainUsageMax <> Null then PainUsage = PainUsageMin
	USHORT  PainUsageMax; 
	// XXUsage servirá para definir los actuadores multiplexados.

	// PainCurve es la curva que relaciona el valor del sensor respecto de las zonas de dolor
	// El algoritmo que utilice la curva PainCurve tiene la obligación de buscar la posición de mínimo dolor del valor

	USHORT	 PainCurveType;
	FLOAT    PainLogicalMin;    
	FLOAT    PainLogicalMax;
	FLOAT    PainLimitR;
	FLOAT    PainLimitM;
	FLOAT    PainLimitL;
	FLOAT    PainAux;

	// joy curve is la curva que relaciona el valor del sensor respecto del parámetro físico
	// Al contrario que en la curva PainCurve, el algoritmo que utilice la curva joycurvetype no tiene
	// obligación de llevar el valor a la posición máxima de la curva aunque esta sería la posición preferida.

	// Son los valores límites extraidos del sensor sin decimales o unidades extrañas
	USHORT	 JoyCurveType;
	FLOAT    JoyLogicalMin;    
	FLOAT    JoyLogicalMax;
	FLOAT    JoyLimitR;
	FLOAT    JoyLimitM;
	FLOAT    JoyLimitL;
	FLOAT    JoyAux;
	
	FLOAT    JoyRawValue;	// Joy raw value to be captured. Joy curve must be applied outside
	FLOAT    PainRawValue;	// Pain value to be captured. Joy curve must be applied outside
	FLOAT    JoyValue;	// Joy value to be captured
	FLOAT    PainValue;	// Pain value to be captured

	__timeb64 AdquiredAt; // time reference for the value captured.

} HIDS_ENDOSENSOR, *PHIDS_ENDOSENSOR;

typedef struct _HIDS_ENDODRIVER
{

    USHORT  DriverID;
	UCHAR   DriverName[100];
	USHORT  VendorID;
    USHORT  ProductID;
	USHORT  VersionNumber;
    USHORT  UsagePage;
    USHORT  Usage;			// If UsageMax <> Null then Usage = UsageMin
    USHORT  UsageMax; 

	// Son los valores límites extraidos del sensor sin decimales o unidades extrañas
	FLOAT    DrvLogicalMin;    
	FLOAT    DrvLogicalMax;

	FLOAT    DrvRawValue;	// Joy raw value to be captured. Joy curve must be applied outside
	FLOAT    DrvValue;	// Joy value to be captured

	struct __timeb64 DrivedAt; // time reference for the value captured.

} HIDS_ENDODRIVER, *PHIDS_ENDODRIVER;

#endif //  __HID_ESENSOR_H__


