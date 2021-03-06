#ifndef STAMPSCONTROLLER_H_
#define STAMPSCONTROLLER_H_

#include "common/String.h"

class ControllerCallback;
class SaveFile;
class LocalBrowserView;
class LocalBrowserModel;
class LocalBrowserController {
	ControllerCallback * callback;
	LocalBrowserView * browserView;
	LocalBrowserModel * browserModel;
public:
	bool HasDone;
	LocalBrowserController(ControllerCallback * callback);
	LocalBrowserView * GetView() {return browserView;}
	SaveFile * GetSave();
	void RemoveSelected();
	void removeSelectedC();
	void ClearSelection();
	void Selected(ByteString stampID, bool selected);
	void RescanStamps();
	void rescanStampsC();
	void RefreshSavesList();
	void OpenSave(SaveFile * stamp);
	bool GetMoveToFront();
	void SetMoveToFront(bool move);
	void SetPage(int page);
	void SetPageRelative(int offset);
	void Update();
	void Exit();
	virtual ~LocalBrowserController();
};

#endif /* STAMPSCONTROLLER_H_ */
