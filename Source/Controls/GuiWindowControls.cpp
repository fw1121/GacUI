#include "GuiWindowControls.h"
#include "GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;
			using namespace compositions;
			using namespace collections;
			using namespace reflection::description;

/***********************************************************************
GuiControlHost
***********************************************************************/

			void GuiControlHost::OnNativeWindowChanged()
			{
			}

			void GuiControlHost::OnVisualStatusChanged()
			{
			}

			controls::GuiControlHost* GuiControlHost::GetControlHostForInstance()
			{
				return this;
			}

			GuiControl* GuiControlHost::GetTooltipOwner(Point location)
			{
				GuiGraphicsComposition* composition=this->GetBoundsComposition()->FindComposition(location, false);
				if(composition)
				{
					GuiControl* control=composition->GetRelatedControl();
					while(control)
					{
						if(control->GetTooltipControl())
						{
							return control;
						}
						control=control->GetParent();
					}
				}
				return nullptr;
			}

			void GuiControlHost::MoveIntoTooltipControl(GuiControl* tooltipControl, Point location)
			{
				if(tooltipLocation!=location)
				{
					tooltipLocation=location;
					{
						GuiControl* currentOwner=GetApplication()->GetTooltipOwner();
						if(currentOwner && currentOwner!=tooltipControl)
						{
							if(tooltipCloseDelay)
							{
								tooltipCloseDelay->Cancel();
								tooltipCloseDelay=0;
							}
							GetApplication()->DelayExecuteInMainThread([=]()
							{
								currentOwner->CloseTooltip();
							}, TooltipDelayCloseTime);
						}
					}
					if(!tooltipControl)
					{
						if(tooltipOpenDelay)
						{
							tooltipOpenDelay->Cancel();
							tooltipOpenDelay=0;
						}
					}
					else if(tooltipOpenDelay)
					{
						tooltipOpenDelay->Delay(TooltipDelayOpenTime);
					}
					else if(GetApplication()->GetTooltipOwner()!=tooltipControl)
					{
						tooltipOpenDelay=GetApplication()->DelayExecuteInMainThread([this]()
						{
							GuiControl* owner=GetTooltipOwner(tooltipLocation);
							if(owner)
							{
								Point offset=owner->GetBoundsComposition()->GetGlobalBounds().LeftTop();
								Point p(tooltipLocation.x-offset.x, tooltipLocation.y-offset.y+24);
								owner->DisplayTooltip(p);
								tooltipOpenDelay=0;

								tooltipCloseDelay=GetApplication()->DelayExecuteInMainThread([this, owner]()
								{
									owner->CloseTooltip();
								}, TooltipDelayLifeTime);
							}
						}, TooltipDelayOpenTime);
					}
				}
			}

			void GuiControlHost::MouseMoving(const NativeWindowMouseInfo& info)
			{
				if(!info.left && !info.middle && !info.right)
				{
					GuiControl* tooltipControl=GetTooltipOwner(tooltipLocation);
					MoveIntoTooltipControl(tooltipControl, Point(info.x, info.y));
				}
			}

			void GuiControlHost::MouseLeaved()
			{
				MoveIntoTooltipControl(0, Point(-1, -1));
			}

			void GuiControlHost::Moved()
			{
				OnVisualStatusChanged();
			}

			void GuiControlHost::Enabled()
			{
				GuiControl::SetEnabled(true);
				OnVisualStatusChanged();
			}

			void GuiControlHost::Disabled()
			{
				GuiControl::SetEnabled(false);
				OnVisualStatusChanged();
			}

			void GuiControlHost::GotFocus()
			{
				WindowGotFocus.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::LostFocus()
			{
				WindowLostFocus.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Activated()
			{
				WindowActivated.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Deactivated()
			{
				WindowDeactivated.Execute(GetNotifyEventArguments());
				OnVisualStatusChanged();
			}

			void GuiControlHost::Opened()
			{
				WindowOpened.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Closing(bool& cancel)
			{
				GuiRequestEventArgs arguments(boundsComposition);
				arguments.cancel=cancel;
				WindowClosing.Execute(arguments);
				if(!arguments.handled)
				{
					cancel=arguments.cancel;
				}
			}

			void GuiControlHost::Closed()
			{
				WindowClosed.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Destroying()
			{
				WindowDestroying.Execute(GetNotifyEventArguments());
				calledDestroyed = true;
				if (deleteWhenDestroyed)
				{
					GetApplication()->InvokeInMainThread(this, [=]()
					{
						delete this;
					});
				}
				SetNativeWindow(nullptr);
			}

			void GuiControlHost::UpdateClientSizeAfterRendering(Size clientSize)
			{
				SetClientSize(clientSize);
			}

			GuiControlHost::GuiControlHost(theme::ThemeName themeName)
				:GuiControl(themeName)
			{
				boundsComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				
				WindowGotFocus.SetAssociatedComposition(boundsComposition);
				WindowLostFocus.SetAssociatedComposition(boundsComposition);
				WindowActivated.SetAssociatedComposition(boundsComposition);
				WindowDeactivated.SetAssociatedComposition(boundsComposition);
				WindowOpened.SetAssociatedComposition(boundsComposition);
				WindowClosing.SetAssociatedComposition(boundsComposition);
				WindowClosed.SetAssociatedComposition(boundsComposition);
				WindowDestroying.SetAssociatedComposition(boundsComposition);

				host=new GuiGraphicsHost(this, boundsComposition);
				sharedPtrDestructorProc = 0;
			}

			GuiControlHost::~GuiControlHost()
			{
				FinalizeInstanceRecursively(this);
				OnBeforeReleaseGraphicsHost();
				delete host;
			}

			void GuiControlHost::DeleteAfterProcessingAllEvents()
			{
				auto window = host->GetNativeWindow();
				if (calledDestroyed || !window)
				{
					delete this;
				}
				else
				{
					deleteWhenDestroyed = true;
					GetCurrentController()->WindowService()->DestroyNativeWindow(window);
				}
			}

			compositions::GuiGraphicsHost* GuiControlHost::GetGraphicsHost()
			{
				return host;
			}

			compositions::GuiGraphicsComposition* GuiControlHost::GetMainComposition()
			{
				return host->GetMainComposition();
			}

			INativeWindow* GuiControlHost::GetNativeWindow()
			{
				return host->GetNativeWindow();
			}

			void GuiControlHost::SetNativeWindow(INativeWindow* window)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->UninstallListener(this);
				}
				host->SetNativeWindow(window);
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->InstallListener(this);
				}
				OnNativeWindowChanged();
			}

			void GuiControlHost::ForceCalculateSizeImmediately()
			{
				auto bounds = GetBounds();
				boundsComposition->ForceCalculateSizeImmediately();
				SetBounds(bounds);
			}

			bool GuiControlHost::GetEnabled()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsEnabled();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetEnabled(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->Enable();
					}
					else
					{
						host->GetNativeWindow()->Disable();
					}
				}
			}

			bool GuiControlHost::GetFocused()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsFocused();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetFocused()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetFocus();
				}
			}

			bool GuiControlHost::GetActivated()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsActivated();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetActivated()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetActivate();
				}
			}

			bool GuiControlHost::GetShowInTaskBar()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsAppearedInTaskBar();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetShowInTaskBar(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->ShowInTaskBar();
					}
					else
					{
						host->GetNativeWindow()->HideInTaskBar();
					}
				}
			}

			bool GuiControlHost::GetEnabledActivate()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsEnabledActivate();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetEnabledActivate(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->EnableActivate();
					}
					else
					{
						host->GetNativeWindow()->DisableActivate();
					}
				}
			}

			bool GuiControlHost::GetTopMost()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetTopMost();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetTopMost(bool topmost)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTopMost(topmost);
				}
			}

			compositions::IGuiShortcutKeyManager* GuiControlHost::GetShortcutKeyManager()
			{
				return host->GetShortcutKeyManager();
			}

			void GuiControlHost::SetShortcutKeyManager(compositions::IGuiShortcutKeyManager* value)
			{
				host->SetShortcutKeyManager(value);
			}

			compositions::GuiGraphicsTimerManager* GuiControlHost::GetTimerManager()
			{
				return host->GetTimerManager();
			}

			Size GuiControlHost::GetClientSize()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetClientSize();
				}
				else
				{
					return Size(0, 0);
				}
			}

			void GuiControlHost::SetClientSize(Size value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetClientSize(value);
				}
			}

			Rect GuiControlHost::GetBounds()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetBounds();
				}
				else
				{
					return Rect();
				}
			}

			void GuiControlHost::SetBounds(Rect value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetBounds(value);
				}
			}

			GuiControlHost* GuiControlHost::GetRelatedControlHost()
			{
				return this;
			}

			const WString& GuiControlHost::GetText()
			{
				WString result;
				if(host->GetNativeWindow())
				{
					result=host->GetNativeWindow()->GetTitle();
				}
				if(result!=GuiControl::GetText())
				{
					GuiControl::SetText(result);
				}
				return GuiControl::GetText();
			}

			void GuiControlHost::SetText(const WString& value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTitle(value);
					GuiControl::SetText(value);
				}
			}

			INativeScreen* GuiControlHost::GetRelatedScreen()
			{
				if(host->GetNativeWindow())
				{
					return GetCurrentController()->ScreenService()->GetScreen(host->GetNativeWindow());
				}
				else
				{
					return 0;
				}
			}

			void GuiControlHost::Show()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Show();
				}
			}

			void GuiControlHost::ShowDeactivated()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowDeactivated();
				}
			}

			void GuiControlHost::ShowRestored()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowRestored();
				}
			}

			void GuiControlHost::ShowMaximized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMaximized();
				}
			}

			void GuiControlHost::ShowMinimized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMinimized();
				}
			}

			void GuiControlHost::Hide()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Hide(false);
				}
			}

			void GuiControlHost::Close()
			{
				if (auto window = host->GetNativeWindow())
				{
					auto mainWindow = GetCurrentController()->WindowService()->GetMainWindow();
					if (mainWindow == window)
					{
						SetNativeWindow(nullptr);
						GetCurrentController()->WindowService()->DestroyNativeWindow(window);
					}
					else
					{
						window->Hide(false);
					}
				}
			}

			bool GuiControlHost::GetOpening()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					return window->IsVisible();
				}
				return false;
			}

/***********************************************************************
GuiWindow
***********************************************************************/

			void GuiWindow::BeforeControlTemplateUninstalled_()
			{
			}

			void GuiWindow::AfterControlTemplateInstalled_(bool initialize)
			{
				auto ct = GetControlTemplateObject(true);
#define FIX_WINDOW_PROPERTY(VARIABLE, NAME) \
				switch (ct->Get ## NAME ## Option()) \
				{ \
				case templates::BoolOption::AlwaysTrue: \
					VARIABLE = true; \
					break; \
				case templates::BoolOption::AlwaysFalse: \
					VARIABLE = false; \
					break; \
				default:; \
				} \

				FIX_WINDOW_PROPERTY(hasMaximizedBox, MaximizedBox)
				FIX_WINDOW_PROPERTY(hasMinimizedBox, MinimizedBox)
				FIX_WINDOW_PROPERTY(hasBorder, Border)
				FIX_WINDOW_PROPERTY(hasSizeBox, SizeBox)
				FIX_WINDOW_PROPERTY(isIconVisible, IconVisible)
				FIX_WINDOW_PROPERTY(hasTitleBar, TitleBar)

#undef FIX_WINDOW_PROPERTY
				ct->SetMaximizedBox(hasMaximizedBox);
				ct->SetMinimizedBox(hasMinimizedBox);
				ct->SetBorder(hasBorder);
				ct->SetSizeBox(hasSizeBox);
				ct->SetIconVisible(isIconVisible);
				ct->SetTitleBar(hasTitleBar);
				ct->SetMaximized(GetNativeWindow()->GetSizeState() != INativeWindow::Maximized);
				ct->SetActivated(GetActivated());
				ct->SetCustomFramePadding(Margin(8, 8, 8, 8));
				SyncNativeWindowProperties();
			}

			void GuiWindow::SyncNativeWindowProperties()
			{
				if (auto window = GetNativeWindow())
				{
					if (GetControlTemplateObject(true)->GetCustomFrameEnabled())
					{
						window->EnableCustomFrameMode();
						window->SetBorder(false);
					}
					else
					{
						window->DisableCustomFrameMode();
						window->SetBorder(hasBorder);
					}

					window->SetMaximizedBox(hasMaximizedBox);
					window->SetMinimizedBox(hasMinimizedBox);
					window->SetSizeBox(hasSizeBox);
					window->SetIconVisible(isIconVisible);
					window->SetTitleBar(hasTitleBar);
				}
			}

			void GuiWindow::Moved()
			{
				GuiControlHost::Moved();
				GetControlTemplateObject(true)->SetMaximized(GetNativeWindow()->GetSizeState() != INativeWindow::Maximized);
			}

			void GuiWindow::OnNativeWindowChanged()
			{
				SyncNativeWindowProperties();
				GuiControlHost::OnNativeWindowChanged();
			}

			void GuiWindow::OnVisualStatusChanged()
			{
				GuiControlHost::OnVisualStatusChanged();
			}

			void GuiWindow::MouseClickedOnOtherWindow(GuiWindow* window)
			{
			}

			void GuiWindow::OnWindowActivated(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if (auto ct = GetControlTemplateObject(false))
				{
					ct->SetActivated(true);
				}
			}

			void GuiWindow::OnWindowDeactivated(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				if (auto ct = GetControlTemplateObject(false))
				{
					ct->SetActivated(false);
				}
			}

			GuiWindow::GuiWindow(theme::ThemeName themeName)
				:GuiControlHost(themeName)
			{
				SetAltComposition(boundsComposition);
				SetAltControl(this, true);

				INativeWindow* window = GetCurrentController()->WindowService()->CreateNativeWindow();
				SetNativeWindow(window);
				GetApplication()->RegisterWindow(this);
				ClipboardUpdated.SetAssociatedComposition(boundsComposition);

				WindowActivated.AttachMethod(this, &GuiWindow::OnWindowActivated);
				WindowDeactivated.AttachMethod(this, &GuiWindow::OnWindowDeactivated);
			}

			GuiWindow::~GuiWindow()
			{
				FinalizeAggregation();
				GetApplication()->UnregisterWindow(this);
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					SetNativeWindow(nullptr);
					GetCurrentController()->WindowService()->DestroyNativeWindow(window);
				}
			}

			IDescriptable* GuiWindow::QueryService(const WString& identifier)
			{
				if (identifier == IGuiAltActionHost::Identifier)
				{
					return (IGuiAltActionHost*)this;
				}
				else
				{
					return GuiControlHost::QueryService(identifier);
				}
			}

			void GuiWindow::MoveToScreenCenter()
			{
				MoveToScreenCenter(GetRelatedScreen());
			}

			void GuiWindow::MoveToScreenCenter(INativeScreen* screen)
			{
				if (screen)
				{
					Rect screenBounds = screen->GetClientBounds();
					Rect windowBounds = GetBounds();
					SetBounds(
						Rect(
							Point(
								screenBounds.Left() + (screenBounds.Width() - windowBounds.Width()) / 2,
								screenBounds.Top() + (screenBounds.Height() - windowBounds.Height()) / 2
							),
							windowBounds.GetSize()
						)
					);
				}
			}

#define IMPL_WINDOW_PROPERTY(VARIABLE, NAME, CONDITION_BREAK) \
			bool GuiWindow::Get ## NAME() \
			{ \
				return VARIABLE; \
			} \
			void GuiWindow::Set ## NAME(bool visible) \
			{ \
				auto ct = GetControlTemplateObject(true); \
				if (ct->Get ## NAME ## Option() == templates::BoolOption::Customizable) \
				{ \
					VARIABLE = visible; \
					ct->Set ## NAME(visible); \
					if (auto window = GetNativeWindow()) \
					{ \
						CONDITION_BREAK \
						window->Set ## NAME(visible); \
						ct->SetCustomFramePadding(window->GetCustomFramePadding()); \
					} \
					else \
					{ \
						ct->SetCustomFramePadding({}); \
					} \
				} \
			} \

#define IMPL_WINDOW_PROPERTY_EMPTY_CONDITION
#define IMPL_WINDOW_PROPERTY_BORDER_CONDITION if (!ct->GetCustomFrameEnabled())

			IMPL_WINDOW_PROPERTY(hasMaximizedBox, MaximizedBox, IMPL_WINDOW_PROPERTY_EMPTY_CONDITION)
			IMPL_WINDOW_PROPERTY(hasMinimizedBox, MinimizedBox, IMPL_WINDOW_PROPERTY_EMPTY_CONDITION)
			IMPL_WINDOW_PROPERTY(hasBorder, Border, IMPL_WINDOW_PROPERTY_BORDER_CONDITION)
			IMPL_WINDOW_PROPERTY(hasSizeBox, SizeBox, IMPL_WINDOW_PROPERTY_EMPTY_CONDITION)
			IMPL_WINDOW_PROPERTY(isIconVisible, IconVisible, IMPL_WINDOW_PROPERTY_EMPTY_CONDITION)
			IMPL_WINDOW_PROPERTY(hasTitleBar, TitleBar, IMPL_WINDOW_PROPERTY_EMPTY_CONDITION)

#undef IMPL_WINDOW_PROPERTY_BORDER_CONDITION
#undef IMPL_WINDOW_PROPERTY_EMPTY_CONDITION
#undef IMPL_WINDOW_PROPERTY

			void GuiWindow::ShowModal(GuiWindow* owner, const Func<void()>& callback)
			{
				owner->SetEnabled(false);
				GetNativeWindow()->SetParent(owner->GetNativeWindow());
				auto container = MakePtr<IGuiGraphicsEventHandler::Container>();
				container->handler = WindowClosed.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
				{
					GetApplication()->InvokeInMainThread(this, [=]()
					{
						WindowClosed.Detach(container->handler);
						container->handler = nullptr;
						GetNativeWindow()->SetParent(0);
						callback();
						owner->SetEnabled(true);
						owner->SetActivated();
					});
				});
				Show();
			}

			void GuiWindow::ShowModalAndDelete(GuiWindow* owner, const Func<void()>& callback)
			{
				ShowModal(owner, [=]()
				{
					callback();
					DeleteAfterProcessingAllEvents();
				});
			}

			Ptr<reflection::description::IAsync> GuiWindow::ShowModalAsync(GuiWindow* owner)
			{
				auto future = IFuture::Create();
				ShowModal(owner, [promise = future->GetPromise()]()
				{
					promise->SendResult({});
				});
				return future;
			}

/***********************************************************************
GuiPopup
***********************************************************************/

			void GuiPopup::UpdateClientSizeAfterRendering(Size clientSize)
			{
				if (popupType == -1)
				{
					GuiWindow::UpdateClientSizeAfterRendering(clientSize);
				}
				else
				{
					auto window = GetNativeWindow();
					auto position = CalculatePopupPosition(clientSize, popupType, popupInfo);
					window->SetBounds(Rect(position, clientSize));
				}
			}

			void GuiPopup::MouseClickedOnOtherWindow(GuiWindow* window)
			{
				Hide();
			}

			void GuiPopup::PopupOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				GetApplication()->RegisterPopupOpened(this);
			}

			void GuiPopup::PopupClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				popupType = -1;
				GetApplication()->RegisterPopupClosed(this);
				if(auto window = GetNativeWindow())
				{
					window->SetParent(nullptr);
				}
			}

			void GuiPopup::OnKeyDown(compositions::GuiGraphicsComposition* sender, compositions::GuiKeyEventArgs& arguments)
			{
				if (!arguments.handled)
				{
					Hide();
				}
			}

			Point GuiPopup::CalculatePopupPosition(Size size, Point location, INativeScreen* screen)
			{
				Rect screenBounds = screen->GetClientBounds();

				if (location.x < screenBounds.x1)
				{
					location.x = screenBounds.x1;
				}
				else if (location.x + size.x > screenBounds.x2)
				{
					location.x = screenBounds.x2 - size.x;
				}

				if (location.y < screenBounds.y1)
				{
					location.y = screenBounds.y1;
				}
				else if (location.y + size.y > screenBounds.y2)
				{
					location.y = screenBounds.y2 - size.y;
				}

				return location;
			}

			bool GuiPopup::IsClippedByScreen(Size size, Point location, INativeScreen* screen)
			{
				Rect screenBounds = screen->GetClientBounds();
				Rect windowBounds(location, size);
				return !screenBounds.Contains(windowBounds.LeftTop()) || !screenBounds.Contains(windowBounds.RightBottom());
			}

			Point GuiPopup::CalculatePopupPosition(Size size, GuiControl* control, INativeWindow* controlWindow, Rect bounds, bool preferredTopBottomSide)
			{
				Point controlClientOffset = control->GetBoundsComposition()->GetGlobalBounds().LeftTop();
				Point controlWindowOffset = controlWindow->GetClientBoundsInScreen().LeftTop();
				bounds.x1 += controlClientOffset.x + controlWindowOffset.x;
				bounds.x2 += controlClientOffset.x + controlWindowOffset.x;
				bounds.y1 += controlClientOffset.y + controlWindowOffset.y;
				bounds.y2 += controlClientOffset.y + controlWindowOffset.y;

				Point locations[4];
				if (preferredTopBottomSide)
				{
					locations[0] = Point(bounds.x1, bounds.y2);
					locations[1] = Point(bounds.x2 - size.x, bounds.y2);
					locations[2] = Point(bounds.x1, bounds.y1 - size.y);
					locations[3] = Point(bounds.x2 - size.x, bounds.y1 - size.y);
				}
				else
				{
					locations[0] = Point(bounds.x2, bounds.y1);
					locations[1] = Point(bounds.x2, bounds.y2 - size.y);
					locations[2] = Point(bounds.x1 - size.x, bounds.y1);
					locations[3] = Point(bounds.x1 - size.x, bounds.y2 - size.y);
				}

				auto screen = GetCurrentController()->ScreenService()->GetScreen(controlWindow);
				for (vint i = 0; i < 4; i++)
				{
					if (!IsClippedByScreen(size, locations[i], screen))
					{
						return CalculatePopupPosition(size, locations[i], screen);
					}
				}
				return CalculatePopupPosition(size, locations[0], screen);
			}

			Point GuiPopup::CalculatePopupPosition(Size size, GuiControl* control, INativeWindow* controlWindow, Point location)
			{
				Point locations[4];
				Rect controlBounds = control->GetBoundsComposition()->GetGlobalBounds();

				Point controlClientOffset = controlWindow->GetClientBoundsInScreen().LeftTop();
				vint x = controlBounds.x1 + controlClientOffset.x + location.x;
				vint y = controlBounds.y1 + controlClientOffset.y + location.y;
				return CalculatePopupPosition(size, Point(x, y), GetCurrentController()->ScreenService()->GetScreen(controlWindow));
			}

			Point GuiPopup::CalculatePopupPosition(Size size, GuiControl* control, INativeWindow* controlWindow, bool preferredTopBottomSide)
			{
				Rect bounds(Point(0, 0), control->GetBoundsComposition()->GetBounds().GetSize());
				return CalculatePopupPosition(size, control, controlWindow, bounds, preferredTopBottomSide);
			}

			Point GuiPopup::CalculatePopupPosition(Size size, vint popupType, const PopupInfo& popupInfo)
			{
				switch (popupType)
				{
				case 1:
					return CalculatePopupPosition(size, popupInfo._1.location, popupInfo._1.screen);
				case 2:
					return CalculatePopupPosition(size, popupInfo._2.control, popupInfo._2.controlWindow, popupInfo._2.bounds, popupInfo._2.preferredTopBottomSide);
				case 3:
					return CalculatePopupPosition(size, popupInfo._3.control, popupInfo._3.controlWindow, popupInfo._3.location);
				case 4:
					return CalculatePopupPosition(size, popupInfo._4.control, popupInfo._4.controlWindow, popupInfo._4.preferredTopBottomSide);
				default:
					CHECK_FAIL(L"vl::presentation::controls::GuiPopup::CalculatePopupPosition(Size, const PopupInfo&)#Internal error.");
				}
			}

			void GuiPopup::ShowPopupInternal()
			{
				auto window = GetNativeWindow();
				UpdateClientSizeAfterRendering(window->GetBounds().GetSize());
				switch (popupType)
				{
				case 2: window->SetParent(popupInfo._2.controlWindow); break;
				case 3: window->SetParent(popupInfo._3.controlWindow); break;
				case 4: window->SetParent(popupInfo._4.controlWindow); break;
				}
				ShowDeactivated();
			}

			GuiPopup::GuiPopup(theme::ThemeName themeName)
				:GuiWindow(themeName)
			{
				SetMinimizedBox(false);
				SetMaximizedBox(false);
				SetSizeBox(false);
				SetTitleBar(false);
				SetShowInTaskBar(false);

				WindowOpened.AttachMethod(this, &GuiPopup::PopupOpened);
				WindowClosed.AttachMethod(this, &GuiPopup::PopupClosed);
				boundsComposition->GetEventReceiver()->keyDown.AttachMethod(this, &GuiPopup::OnKeyDown);
			}

			GuiPopup::~GuiPopup()
			{
				GetApplication()->RegisterPopupClosed(this);
			}

			void GuiPopup::ShowPopup(Point location, INativeScreen* screen)
			{
				if (auto window = GetNativeWindow())
				{
					if (!screen)
					{
						SetBounds(Rect(location, GetBounds().GetSize()));
						screen = GetCurrentController()->ScreenService()->GetScreen(window);
					}

					popupType = 1;
					popupInfo._1.location = location;
					popupInfo._1.screen = screen;
					ShowPopupInternal();
				}
			}
			
			void GuiPopup::ShowPopup(GuiControl* control, Rect bounds, bool preferredTopBottomSide)
			{
				if (auto window = GetNativeWindow())
				{
					if (auto controlHost = control->GetBoundsComposition()->GetRelatedControlHost())
					{
						if (auto controlWindow = controlHost->GetNativeWindow())
						{
							popupType = 2;
							popupInfo._2.control = control;
							popupInfo._2.controlWindow = controlWindow;
							popupInfo._2.bounds = bounds;
							popupInfo._2.preferredTopBottomSide = preferredTopBottomSide;
							ShowPopupInternal();
						}
					}
				}
			}

			void GuiPopup::ShowPopup(GuiControl* control, Point location)
			{
				if (auto window = GetNativeWindow())
				{
					if (auto controlHost = control->GetBoundsComposition()->GetRelatedControlHost())
					{
						if (auto controlWindow = controlHost->GetNativeWindow())
						{
							popupType = 3;
							popupInfo._3.control = control;
							popupInfo._3.controlWindow = controlWindow;
							popupInfo._3.location = location;
							ShowPopupInternal();
						}
					}
				}
			}

			void GuiPopup::ShowPopup(GuiControl* control, bool preferredTopBottomSide)
			{
				if (auto window = GetNativeWindow())
				{
					if (auto controlHost = control->GetBoundsComposition()->GetRelatedControlHost())
					{
						if (auto controlWindow = controlHost->GetNativeWindow())
						{
							popupType = 4;
							popupInfo._4.control = control;
							popupInfo._4.controlWindow = controlWindow;
							popupInfo._4.preferredTopBottomSide = preferredTopBottomSide;
							ShowPopupInternal();
						}
					}
				}
			}

/***********************************************************************
GuiPopup
***********************************************************************/

			void GuiTooltip::GlobalTimer()
			{
				SetClientSize(GetClientSize());
			}

			void GuiTooltip::TooltipOpened(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
			}

			void GuiTooltip::TooltipClosed(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments)
			{
				SetTemporaryContentControl(0);
			}

			GuiTooltip::GuiTooltip(theme::ThemeName themeName)
				:GuiPopup(themeName)
				,temporaryContentControl(0)
			{
				containerComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				containerComposition->SetPreferredMinSize(Size(20, 10));
				GetCurrentController()->CallbackService()->InstallListener(this);

				WindowOpened.AttachMethod(this, &GuiTooltip::TooltipOpened);
				WindowClosed.AttachMethod(this, &GuiTooltip::TooltipClosed);
			}

			GuiTooltip::~GuiTooltip()
			{
				GetCurrentController()->CallbackService()->UninstallListener(this);
			}

			vint GuiTooltip::GetPreferredContentWidth()
			{
				return containerComposition->GetPreferredMinSize().x;
			}

			void GuiTooltip::SetPreferredContentWidth(vint value)
			{
				containerComposition->SetPreferredMinSize(Size(value, 10));
			}

			GuiControl* GuiTooltip::GetTemporaryContentControl()
			{
				return temporaryContentControl;
			}

			void GuiTooltip::SetTemporaryContentControl(GuiControl* control)
			{
				if(temporaryContentControl && HasChild(temporaryContentControl))
				{
					containerComposition->RemoveChild(temporaryContentControl->GetBoundsComposition());
					temporaryContentControl=0;
				}
				temporaryContentControl=control;
				if(control)
				{
					control->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					AddChild(control);
				}
			}
		}
	}
}
