#include "stdafx.h"
#include "customControlFactory.h"

#include "../page/testControlPage.h"
#include "../page/base64Page.h"
#include "../page/blockcalcPage.h"
#include "../frame/caption.h"
#include "../frame/toolbar.h"
//#include "../mathGraphic/glwindow.h"
#include "../page/stockDataPage.h"
#include "../Control/stockDataTable.h"

CCustomControlFactory::CCustomControlFactory()
{
	REGIST_DUICONTROL(CToolbarPageUI);
	REGIST_DUICONTROL(CCaptionPageUI);
	REGIST_DUICONTROL(CTestControlPageUI);
 	REGIST_DUICONTROL(CBasePageUI);
 	REGIST_DUICONTROL(CBase64PageUI);
 	REGIST_DUICONTROL(CBlockcalcPageUI);
	//REGIST_DUICONTROL(CGLUI);
	REGIST_DUICONTROL(CStockDataPageUI);
	REGIST_DUICONTROL(CStockDataTableUI);
	REGIST_DUICONTROL(CStockDataTableHeaderUI);
}

void CCustomControlFactory::InitFactory()
{
	static CCustomControlFactory s_obj;
}