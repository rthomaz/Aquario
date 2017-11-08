#ifndef DisplayWiFiManager_h
#define DisplayWiFiManager_h

#include "Arduino.h"
#include "DebugManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"

#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold9pt7b.h"

class DisplayWiFiManager
{
public:
	DisplayWiFiManager(DisplayManager& displayManager, WiFiManager& wifiManager, DebugManager& debugManager);
	~DisplayWiFiManager();	

	void 							printSignal ();
	
private:

	DisplayManager*       			_displayManager;	
	WiFiManager*          			_wifiManager;
	DebugManager*         			_debugManager;
			
	bool 							_firstTimecaptivePortalCallback = true;
			
	void							printPortalHeaderInDisplay(String title);
	void 							showEnteringSetup();
	void 							showWiFiConect();
		
	void							printConnectedSignal(int x, int y, int barWidth, int margin, int barSignal);
	void							printNoConnectedSignal(int x, int y, int barWidth, int margin);

	void 							startConfigPortalCallback ();
	void 							captivePortalCallback (String ip);
	void 							successConfigPortalCallback ();
	void 							failedConfigPortalCallback (int connectionResult);
	void 							connectingConfigPortalCallback ();
	
	std::function<void(void)> 		_startConfigPortalCallback;
	std::function<void(String)>		_captivePortalCallback;
	std::function<void(void)> 		_successConfigPortalCallback;
	std::function<void(int)>		_failedConfigPortalCallback;
	std::function<void(void)> 		_connectingConfigPortalCallback;
	
};

#endif