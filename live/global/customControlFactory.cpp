#include "stdafx.h"
#include "customControlFactory.h"

#include "../page/testControlPage.h"
#include "../page/base64Page.h"
#include "../stock/blockcalcPage.h"
#include "../frame/caption.h"
#include "../frame/toolbar.h"
#include "../mathGraphic/glwindow.h"
#include "../stock/stockDataPage.h"
#include "../stock/stockDataTable.h"
#include "../browser/browserwindow.h"
#include "../mathGraphic/glwindow.h"
#include "../ImageLive/imageShowPage.h"

CCustomControlFactory::CCustomControlFactory()
{
	REGIST_DUICONTROL(CToolbarPageUI);
	REGIST_DUICONTROL(CCaptionPageUI);
	REGIST_DUICONTROL(CTestControlPageUI);
 	REGIST_DUICONTROL(CBasePageUI);
 	REGIST_DUICONTROL(CBase64PageUI);
 	REGIST_DUICONTROL(CBlockcalcPageUI);
	REGIST_DUICONTROL(CGLUI);
	REGIST_DUICONTROL(CGLPageUI);
	REGIST_DUICONTROL(CStockDataPageUI);
	REGIST_DUICONTROL(CStockDataTableUI);
	REGIST_DUICONTROL(CStockDataTableHeaderUI);
	REGIST_DUICONTROL(CBrowserUI);
	REGIST_DUICONTROL(CToolbarItemUI);
	REGIST_DUICONTROL(CWebBrowserPageUI);
	REGIST_DUICONTROL(CImageShowPageUI);
}

void CCustomControlFactory::InitFactory()
{
	static CCustomControlFactory s_obj;
}