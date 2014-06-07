

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
	// estos seis par�metros identifican un sensor del resto, tambien sirven para diferenciar sensores digitales
	// codificados como botones de sensores anal�gicos e incluso de sensores digitales de salida  codificados como LEDs
	// Aunque se diferencian las se�ales Joy y Pain, puede utilizarse un mismo sensor para extraer cada se�al
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
	// XXUsage servir� para definir los actuadores multiplexados.

	// PainCurve es la curva que relaciona el valor del sensor respecto de las zonas de dolor
	// El algoritmo que utilice la curva PainCurve tiene la obligaci�n de buscar la posici�n de m�nimo dolor del valor

	USHORT	 PainCurveType;
	FLOAT    PainLogicalMin;    
	FLOAT    PainLogicalMax;
	FLOAT    PainLimitR;
	FLOAT    PainLimitM;
	FLOAT    PainLimitL;
	FLOAT    PainAux;

	// joy curve is la curva que relaciona el valor del sensor respecto del par�metro f�sico
	// Al contrario que en la curva PainCurve, el algoritmo que utilice la curva joycurvetype no tiene
	// obligaci�n de llevar el valor a la posici�n m�xima de la curva aunque esta ser�a la posici�n preferida.

	// Son los valores l�mites extraidos del sensor sin decimales o unidades extra�as
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

	// Son los valores l�mites extraidos del sensor sin decimales o unidades extra�as
	FLOAT    DrvLogicalMin;    
	FLOAT    DrvLogicalMax;

	FLOAT    DrvRawValue;	// Joy raw value to be captured. Joy curve must be applied outside
	FLOAT    DrvValue;	// Joy value to be captured

	struct __timeb64 DrivedAt; // time reference for the value captured.

} HIDS_ENDODRIVER, *PHIDS_ENDODRIVER;

#endif //  __HID_ESENSOR_H__


