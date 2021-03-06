#ifndef RENDERVIEW_H_
#define RENDERVIEW_H_

#include <vector>
#include "gui/interface/Window.h"

namespace ui
{
	class Checkbox;
}

class Renderer;
class RenderController;
class RenderModel;
class RenderView: public ui::Window {
	RenderController * c;
	Renderer * ren;
	std::vector<ui::Checkbox*> renderModes;
	std::vector<ui::Checkbox*> displayModes;
	std::vector<ui::Checkbox*> colourModes;
	String toolTip;
	int toolTipPresence;
	bool isToolTipFadingIn;
	int line1, line2, line3, line4;
public:
	class RenderModeAction;
	class DisplayModeAction;
	class ColourModeAction;
	class RenderPresetAction;
	RenderView();
	void NotifyRendererChanged(RenderModel * sender);
	void NotifyRenderChanged(RenderModel * sender);
	void NotifyDisplayChanged(RenderModel * sender);
	void NotifyColourChanged(RenderModel * sender);
	void AttachController(RenderController * c_) { c = c_; }
	void OnMouseDown(int x, int y, unsigned button) override;
	void OnTryExit(ExitMethod method) override;
	void OnDraw() override;
	void OnTick(float dt) override;
	void OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt) override;
	void ToolTip(ui::Point senderPosition, String toolTip) override;
	virtual ~RenderView();
};

#endif /* RENDERVIEW_H_ */
