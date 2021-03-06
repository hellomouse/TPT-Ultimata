#include "LocalBrowserController.h"

#include "LocalBrowserModel.h"
#include "LocalBrowserView.h"

#include "client/Client.h"
#include "gui/dialogues/ConfirmPrompt.h"
#include "tasks/TaskWindow.h"
#include "tasks/Task.h"

#include "Controller.h"

#include "common/tpt-minmax.h"

LocalBrowserController::LocalBrowserController(ControllerCallback * callback):
	HasDone(false)
{
	browserModel = new LocalBrowserModel();
	browserView = new LocalBrowserView();
	browserView->AttachController(this);
	browserModel->AddObserver(browserView);

	this->callback = callback;

	browserModel->UpdateSavesList(1);
}

void LocalBrowserController::OpenSave(SaveFile * save)
{
	browserModel->SetSave(save);
}

SaveFile * LocalBrowserController::GetSave()
{
	return browserModel->GetSave();
}

void LocalBrowserController::RemoveSelected()
{
	class RemoveSelectedConfirmation: public ConfirmDialogueCallback {
	public:
		LocalBrowserController * c;
		RemoveSelectedConfirmation(LocalBrowserController * c_) {	c = c_;	}
		void ConfirmCallback(ConfirmPrompt::DialogueResult result) override {
			if (result == ConfirmPrompt::ResultOkay)
				c->removeSelectedC();
		}
		virtual ~RemoveSelectedConfirmation() { }
	};

	StringBuilder desc;
	desc << "Are you sure you want to delete " << browserModel->GetSelected().size() << " stamp";
	if(browserModel->GetSelected().size()>1)
		desc << "s";
	desc << "?";
	new ConfirmPrompt("Delete stamps", desc.Build(), new RemoveSelectedConfirmation(this));
}

void LocalBrowserController::removeSelectedC()
{
	class RemoveSavesTask : public Task
	{
		std::vector<ByteString> saves;
		LocalBrowserController * c;
	public:
		RemoveSavesTask(LocalBrowserController * c, std::vector<ByteString> saves_) : c(c) { saves = saves_; }
		bool doWork() override
		{
			for (size_t i = 0; i < saves.size(); i++)
			{
				notifyStatus(String::Build("Deleting stamp [", saves[i].FromUtf8(), "] ..."));
				Client::Ref().DeleteStamp(saves[i]);
				notifyProgress((float(i+1)/float(saves.size())*100));
			}
			return true;
		}
		void after() override
		{
			Client::Ref().updateStamps();
			c->RefreshSavesList();
		}
	};

	std::vector<ByteString> selected = browserModel->GetSelected();
	new TaskWindow("Removing stamps", new RemoveSavesTask(this, selected));
}

void LocalBrowserController::RescanStamps()
{
	class RescanConfirmation: public ConfirmDialogueCallback {
	public:
		LocalBrowserController * c;
		RescanConfirmation(LocalBrowserController * c_) {	c = c_;	}
		void ConfirmCallback(ConfirmPrompt::DialogueResult result) override {
			if (result == ConfirmPrompt::ResultOkay)
				c->rescanStampsC();
		}
		virtual ~RescanConfirmation() { }
	};

	String desc = "Rescanning the stamps folder can find stamps added to the stamps folder or recover stamps when the stamps.def file has been lost or damaged. However, be warned that this will mess up the current sorting order";
	new ConfirmPrompt("Rescan", desc, new RescanConfirmation(this));
}

void LocalBrowserController::rescanStampsC()
{
	browserModel->RescanStamps();
	browserModel->UpdateSavesList(browserModel->GetPageNum());
}

void LocalBrowserController::RefreshSavesList()
{
	ClearSelection();
	browserModel->UpdateSavesList(browserModel->GetPageNum());
}

void LocalBrowserController::ClearSelection()
{
	browserModel->ClearSelected();
}

void LocalBrowserController::SetPage(int page)
{
	if (page != browserModel->GetPageNum() && page > 0 && page <= browserModel->GetPageCount())
		browserModel->UpdateSavesList(page);
}

void LocalBrowserController::SetPageRelative(int offset)
{
	int page = std::min(std::max(browserModel->GetPageNum() + offset, 1), browserModel->GetPageCount());
	if (page != browserModel->GetPageNum())
		browserModel->UpdateSavesList(page);
}

void LocalBrowserController::Update()
{
	if(browserModel->GetSave())
	{
		Exit();
	}
}

void LocalBrowserController::Selected(ByteString saveName, bool selected)
{
	if(selected)
		browserModel->SelectSave(saveName);
	else
		browserModel->DeselectSave(saveName);
}

bool LocalBrowserController::GetMoveToFront()
{
	return browserModel->GetMoveToFront();
}

void LocalBrowserController::SetMoveToFront(bool move)
{
	browserModel->SetMoveToFront(move);
}

void LocalBrowserController::Exit()
{
	browserView->CloseActiveWindow();
	if(callback)
		callback->ControllerExit();
	HasDone = true;
}

LocalBrowserController::~LocalBrowserController()
{
	browserView->CloseActiveWindow();
	delete callback;
	delete browserModel;
	delete browserView;
}

