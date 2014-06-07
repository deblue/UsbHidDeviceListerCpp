// RobotDogD1_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "string.h"
#include "RobotDogD1DLG.h"
#include "hid_esensor.h"
#include "hidusage.h"
#include "RDDefault.h"
#include "RobotDogD1.h"
#include "UsbHidIO.h"



// CRobotDogD1DLG dialog

IMPLEMENT_DYNAMIC(CRobotDogD1DLG, CDialog)

CRobotDogD1DLG::CRobotDogD1DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CRobotDogD1DLG::IDD, pParent)
	, m_ebAdquiredAt(_T(""))
	, m_strOutputReport(_T(""))
{

}

CRobotDogD1DLG::~CRobotDogD1DLG()
{
}

void CRobotDogD1DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROBOT_ON, m_Robot_On);
	DDX_Control(pDX, IDC_LISTDEVICES, m_ctlLoadHIDList);
	DDX_Control(pDX, IDC_EDREPORT, m_ebOutputReport);
	DDX_Text(pDX, IDC_EDREPORT, m_strOutputReport);
}
BOOL CRobotDogD1DLG::OnInitDialog()
{
	CDialog::OnInitDialog();
		
		NumHIDDevices = 0;
		StrUsageTableInit = 20000;
		MyDevPath = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(MAXDEVICEPATH);
		MyDevPath->cbSize = 0;
		
		
	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CRobotDogD1DLG::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BEGIN_MESSAGE_MAP(CRobotDogD1DLG, CDialog)
	ON_BN_CLICKED(IDC_ROBOT_ON, &CRobotDogD1DLG::OnBnClickedRobotOn)
		ON_BN_CLICKED(IDC_WRITEDEVREPORT, &CRobotDogD1DLG::OnBnClickedWriteDevReport)
	ON_LBN_SELCHANGE(IDC_LISTDEVICES, &CRobotDogD1DLG::OnLbnSelchangeListDevices)
	ON_WM_CLOSE()	
	ON_MESSAGE(WM_DEVICECHANGE, Main_OnDeviceChange)
	ON_EN_UPDATE(IDC_EDREPORT, &CRobotDogD1DLG::OnEnUpdateEdreport)
	
	ON_WM_VSCROLL()
	
END_MESSAGE_MAP()





void CRobotDogD1DLG::OnBnClickedRobotOn()
{
			LONG				Result;
			CString csVendorID;
			CString csProductID;
			CString csDescription, csProductName;
			wchar_t strProduct[125], strSerialNum[125], strManufacturer[125]; // maximun 125 This string is a wide-character string
			char str2[125];
			int i, count, tmpint;
			size_t tmpSizeT;
			HANDLE AuxDeviceHandle;


			NumHIDDevices = 0;

// getting the list of HID devices
	Result = mUsbHidIO.GetHIDCollectionDevices( NumHIDDevices, arrayDetailData, arrayAttributes, arrayValueCaps);

//	RegisterForDeviceNotifications(m_hWnd);

	m_ctlLoadHIDList.ResetContent();

	for (i=0; i<NumHIDDevices;i++)
	{
	for (count=0; count< 125;count++) strProduct[count]=0;
	for (count=0; count< 125;count++) str2[count]=0;
	csDescription.Empty();
	csProductName.Empty();

	csVendorID.Format("%02X", arrayAttributes[i].VendorID);
	csVendorID = csVendorID.Right(4);	
	csProductID.Format("%02X", arrayAttributes[i].ProductID);
	csProductID = csProductID.Right(4);	
	csDescription = _T("VID:0x") + csVendorID + _T(" PID:0x") + csProductID;

	tmpint = (int)(StrUsageTableInit + 1000 + arrayValueCaps[i].Usage);
	LoadString(GetModuleHandle(NULL), tmpint, str2, sizeof(str2)/sizeof(char)); 
	csDescription = csDescription + _T(" - ") + _T(str2);

	//BEGIN: getting some interesting information about the device -------------
		AuxDeviceHandle=CreateFile (arrayDetailData[i]->DevicePath, 
				0, FILE_SHARE_READ|FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 	0, NULL);
		// This function retrieves the product string
		HidD_GetProductString(AuxDeviceHandle,strProduct,sizeof(strProduct)); 
		// This function retrieves the serial number string
		HidD_GetSerialNumberString (AuxDeviceHandle,strSerialNum,sizeof(strSerialNum) ); 
		//This function retrieves the manufacturer string 
		HidD_GetManufacturerString (AuxDeviceHandle,strManufacturer,sizeof(strManufacturer) ); 
		CloseHandle(AuxDeviceHandle);
	//END: getting some interesting information about the device -------------

	// 
		if (wcstombs_s (&tmpSizeT, str2,(size_t) 125,strProduct,_TRUNCATE) != -1)
		{
			if (tmpSizeT > 2)
			{
				csProductName.Format(" - Product:  %s",str2);   
				csDescription = csDescription + csProductName;
			}
			else
			{
				csDescription = csDescription + _T(" - NO DESCRIPTION ");
			}
		}
	m_ctlLoadHIDList.AddString(csDescription);
	}
}









void CRobotDogD1DLG::OnBnClickedWriteDevReport()
{

	LONG				Result;

	char OutputReport[32];
	char chrTmp[3];
	UCHAR valor;
	char *stopstring;
	int i;

	if (NumHIDDevices <1)
	{
		OnBnClickedRobotOn();
		if (NumHIDDevices < 1)
		{
			MessageBox("Device List Not Ready");
			return;
		}
		else
		{
			OnLbnSelchangeListDevices();
		}
	}

	m_ebOutputReport.GetWindowTextA(m_strOutputReport);
	m_strOutputReport.MakeUpper();
	memset (OutputReport, (char) 0, 32);

		for (i=0; i<m_strOutputReport.GetLength();i+=2)
		{
			chrTmp[0] = m_strOutputReport.GetAt(i);
			chrTmp[1] = m_strOutputReport.GetAt(i+1);
			chrTmp[2] = 0;
			valor = (char)strtoul(chrTmp,&stopstring,16);
			OutputReport[(i/2)+1]= valor;
		}

// Writing Report ------------------------------------
	Result = mUsbHidIO.SendHIDReport (MyDevPath, OutputReport, NULL);

// Cleaning all memory blocks
}

void CRobotDogD1DLG::CloseHandles()
	{
		
//	if (DeviceHandle != INVALID_HANDLE_VALUE)
//		{
//		CloseHandle(DeviceHandle);
//		}
		if(MyDevPath->cbSize > 0 && MyDevPath->cbSize <255) free(MyDevPath);
}

void CRobotDogD1DLG::OnLbnSelchangeListDevices()
{
int tmpIndex;
int PathLeght;

	tmpIndex = m_ctlLoadHIDList.GetCurSel();

	if (tmpIndex == CB_ERR) tmpIndex=0; // CB_ERR if no item is selected.

	MyDevPath->cbSize = arrayDetailData[tmpIndex]->cbSize;
	PathLeght = arrayDetailData[tmpIndex]->DevicePath[0]; //First byt is the leght
	memcpy(MyDevPath->DevicePath,
			arrayDetailData[tmpIndex]->DevicePath, PathLeght);

	MyDevAttrib = arrayAttributes[tmpIndex];
	MyDevCaps = arrayValueCaps[tmpIndex];
		
	MyDevPathAux = true;

}

LRESULT CRobotDogD1DLG::Main_OnDeviceChange(WPARAM wParam, LPARAM lParam)  
	{
  
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;

	switch(wParam) 
		{
		// Find out if a device has been attached or removed.
		// If yes, see if the name matches the device path name of the device we want to access.

		case DBT_DEVICEARRIVAL:
		//A device has been attached.
			mUsbHidIO.DeviceDetected = TRUE;
			MessageBox("My Device has been attached",_T("HID Device"),MB_ICONINFORMATION);
			return TRUE; 
	
		case DBT_DEVICEREMOVECOMPLETE:
		// Look for the device on the next transfer attempt.
			mUsbHidIO.DeviceDetected = FALSE;
			MessageBox("My Device has been disconnected",_T("HID Device"),MB_ICONWARNING);
			return TRUE; 
	
		default:
			return TRUE; 
		} 
}
void CRobotDogD1DLG::RegisterForDeviceNotifications(HWND myWnd)
{

	// Request to receive messages when a device is attached or removed.
	// Also see WM_DEVICECHANGE in BEGIN_MESSAGE_MAP(CUsbhidiocDlg, CDialog).

	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = mUsbHidIO.HidGuid;

	DeviceNotificationHandle =
		RegisterDeviceNotification(myWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

}




void CRobotDogD1DLG::OnEnUpdateEdreport()
{
	LONG				Result;

	char OutputReport[32];
	char chrTmp[3];
	UCHAR valor;
	char *stopstring;
	int i;

	if (NumHIDDevices <1)
	{
		OnBnClickedRobotOn();
		if (NumHIDDevices < 1)
		{
			MessageBox("Device List Not Ready");
			return;
		}
		else
		{
			OnLbnSelchangeListDevices();
		}
	}

	m_ebOutputReport.GetWindowTextA(m_strOutputReport);
	m_strOutputReport.MakeUpper();
	memset (OutputReport, (char) 0, 32);

		for (i=0; i<m_strOutputReport.GetLength();i+=2)
		{
			chrTmp[0] = m_strOutputReport.GetAt(i);
			chrTmp[1] = m_strOutputReport.GetAt(i+1);
			chrTmp[2] = 0;
			valor = (char)strtoul(chrTmp,&stopstring,16);
			OutputReport[(i/2)+1]= valor;
		}
		
// Writing Report ------------------------------------
	Result = mUsbHidIO.SendHIDReport (MyDevPath, OutputReport, NULL);

// Cleaning all memory blocks

//#1015

	// TODO:  Add your control notification handler code here
}






void CRobotDogD1DLG::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


