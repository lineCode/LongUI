﻿#include <luiconf.h>
// ui
#include <interface/ui_default_config.h>
#include <interface/ui_ctrlinfolist.h>
#include <container/pod_vector.h>
#include <core/ui_time_capsule.h>
#include <container/pod_hash.h>
#include <filesystem/ui_file.h>
#include <graphics/ui_dcomp.h>
#include <input/ui_kminput.h>
#include <util/ui_ctordtor.h>
#include <core/ui_manager.h>
#include <core/ui_string.h>
#include <util/ui_ostype.h>
#include <util/ui_endian.h>

// c++
#include <type_traits>
#include <new>

// windows com
#include <objbase.h>

/// <summary>
/// Thises the manager.
/// </summary>
/// <returns></returns>
inline auto LongUI::CUIManager::this_() noexcept -> CUIManager* {
#ifdef NDEBUG
    return &UIManager;
#else
    return this;
#endif
}

// private manager
namespace LongUI { struct CUIManager::Private {
    // mark winmin
    static void MarkWndMin() noexcept { UIManager.m_flagWndMinSizeChanged = true; }
    // TC
    using TC = CUITimeCapsule;
    // meta
    using META = const MetaControl;
    // ctor
    Private() noexcept {  }
    // dtor
    ~Private() noexcept {}
    // time capsules
    POD::Vector<TC*>        time_capsules;
    // unique text
    POD::HashMap<void*>     texts;
    // unique control classes
    POD::HashMap<META*>     cclasses;
    // km-input
    CUIInputKM              km_input;

};}


/// <summary>
/// Sets the CSS root dir.
/// </summary>
/// <param name="dir">The dir.</param>
/// <returns></returns>
//void LongUI::CUIManager::SetCSSRootDir(const wchar_t* dir) noexcept {
//    auto& string = this_()->pm().css_root_dir;
//    string = dir;
//    if (string.empty()) return;
//    const auto index = string.length() - 1;
//    if (string[index] == '/' || string[index] == '\\') return;
//    string.append('/');
//}

// delete later
namespace LongUI { enum CallLater : uint32_t {
    Later_First = WM_USER + 10,
    Later_DeleteControl,
    Later_CallTimeCapsule
};}

/// <summary>
/// Instances this instance.
/// </summary>
/// <returns></returns>
auto LongUI::CUIInputKM::Instance() noexcept -> CUIInputKM & {
    return UIManager.pm().km_input;
}


namespace LongUI { 
    // add default control class
    void AddDefaultControlInfo(ControlInfoList&) noexcept;
    // delete control
    void DeleteControl(UIControl*) noexcept;
    // mark control dl?
    void OnTimer(UIControl&, uint32_t id) noexcept;
    // check control dl?
    bool CheckControlDeleteLater(const UIControl&) noexcept;
    // mark control dl?
    void MarkControlDeleteLater(UIControl&) noexcept;
}

#ifndef NDEBUG
#include <util/ui_time_meter.h>
static void ui_endian_runtime_assert() noexcept;
void ui_dbg_set_window_title(LongUI::CUIWindow*, const char*) noexcept;
void ui_dbg_set_window_title(HWND, const char*) noexcept;

namespace LongUI {
    // debug data
    struct CUIManagerDebug {
        // ctor
        CUIManagerDebug() noexcept {}
        // dtor
        ~CUIManagerDebug() noexcept {}
        // next frame
        void NextFrame() noexcept { ++fcounter; }
        // delta time list
        POD::Vector<float>      fpsc;
        // frame counter
        uint32_t                fcounter = 0;
        // push delta
        auto PushDelta(float d, uint32_t f) noexcept {
            constexpr float add_one_sec = 0.233f * 2.33f * 1.2450f;
            if (const auto size = uint32_t(float(f) * add_one_sec)) {
                fpsc.push_back(d);
                if (fpsc.size() >= size) {
                    float time = 0.f;
                    for (auto t : fpsc) time += t;
                    const auto real_size = float(fpsc.size());
                    time /= real_size;
                    fpsc.clear();
                    return time;
                }
            }
            return 0.f;
        }
    };
    // data buffer
    static std::aligned_storage<
        sizeof(CUIManagerDebug), 
        alignof(CUIManagerDebug)
    >::type s_dbgfManager;
    // get inline
    auto DbgMgr() noexcept->CUIManagerDebug& {
        return reinterpret_cast<CUIManagerDebug&>(s_dbgfManager);
    }
    // get frame id
    auto get_frame_id() noexcept -> uint32_t {
        return DbgMgr().fcounter;
    }

}
#endif


/// <summary>
/// do one frame
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::OneFrame() noexcept {
#ifndef NDEBUG
    CUITimeMeterH meter;
    meter.Start();
    int while_count = 0;
#endif
    // 数据更新区域
    this_()->DataLock();
    // 尝试重新创建
    this_()->try_recreate();
    // 记录帧间时间
    this_()->m_fDeltaTime = this_()->update_delta_time();
    // 刷新控件控制
    this_()->normal_update();
    // 初始化控件
    while (this_()->init_control_in_list() || m_flagWndMinSizeChanged) {
        // 标记为空
        m_flagWndMinSizeChanged = false;
        // 更新窗口最小大小
        this_()->refresh_window_minsize();
        // 刷新在表控件
        this_()->update_control_in_list();
#ifndef NDEBUG
        ++while_count;
        if (while_count == 1000) {
            LUIDebug(Error) << "MAYBE BUG!"
                << LongUI::endl;
        }
#endif
    }
#ifndef NDEBUG
    // 遍历次数>1
    if (while_count > 2) {
        LUIDebug(Hint) LUI_FRAMEID
            << "frame while loop count : "
            << while_count
            << endl;
    }
#endif
    // 次帧刷新列表
    this_()->push_next_update();
    // 更新世界
    this_()->refresh_window_world();
    // 脏矩形更新
    this_()->dirty_update();
    // 记录时间胶囊
    const auto has_tc = this->has_time_capsule();
    // 结束数据更新
    this_()->DataUnlock();
#ifndef NDEBUG
    // 记录刷新时间
    const auto t1 = meter.Delta_ms<float>();
    meter.MovStartEnd();
    //meter.Start();
#endif
    // 时间胶囊S1
    if (has_tc) this->call_time_capsule_s1();
    // 渲染所有窗口
    this_()->before_render_windows();
    this_()->RenderLock();
    const auto hr = this_()->render_windows();
    this_()->RenderUnlock();
    // 时间胶囊S2
    if (has_tc) this->call_time_capsule_s2();
#ifndef NDEBUG
    // 记录渲染时间
    const auto t2 = meter.Delta_ms<float>();
    if (this_()->flag & ConfigFlag::Flag_DbgOutputTimeTook) {
        CUIDataAutoLocker locker;
        LUIDebug(None) LUI_FRAMEID
            << "U<" << DDFFloat2{ t1 } << "ms>"
            << "R<" << DDFFloat2{ t2 } << "ms>"
            << endl;
    }
    else {
        // update超过2毫秒也算
        if (t1 > 2.f) {
            CUIDataAutoLocker locker;
            LUIDebug(Hint) LUI_FRAMEID
                << "U<" << DDFFloat2{ t1 } << "ms>"
                << "R<" << DDFFloat2{ t2 } << "ms>"
                << endl;
        }
    }
    // 计算FPS
    const auto time = DbgMgr().PushDelta(
        this_()->m_fDeltaTime, 
        this_()->m_dwDisplayFrequency
    );
    if (time > 0.f) {
        constexpr size_t buflen = 128;
        char buffer[buflen];
        std::snprintf(
            buffer, buflen,
            "delta: %.2fms -- %2.2f fps",
            time * 1000.f, 1.f / time
        );
        if (this_()->flag & IUIConfigure::Flag_DbgDebugWindow)
            ::ui_dbg_set_window_title(this_()->m_pDebugWindow, buffer);
        else
            ::ui_dbg_set_window_title(this_()->m_hToolWnd, buffer);
    }
    DbgMgr().NextFrame();
#endif
    // TODO: 错误处理
    assert(hr);
}


/// <summary>
/// Waits for v-blank.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::WaitForVBlank() noexcept {
    if (!this_()->wait_for_vblank()) {
        this_()->sleep_for_vblank();
    }
}


/// <summary>
/// Needs the recreate_device.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::NeedRecreate() noexcept {
    CUIDataAutoLocker locker;
    m_flagRecreate = true;
}


/// <summary>
/// Gets the unique text.
/// </summary>
/// <param name="pair">The pair.</param>
/// <returns></returns>
auto LongUI::CUIManager::GetUniqueText(
    U8View pair) noexcept -> const char* {
    assert(pair.second > pair.first && "bad string");
    return this_()->pm().texts.insert(pair.first, pair.second, nullptr).first->first;
}

// windows api
#include <Windows.h>

/// <summary>
/// Creates the control.
/// </summary>
/// <param name="element">The element.</param>
/// <param name="parent">The parent.</param>
/// <returns></returns>
auto LongUI::CUIManager::CreateControl(U8View element,
    UIControl* parent) noexcept -> UIControl* {
    auto& list = this_()->pm().cclasses;
    const auto itr = list.find(element.begin(), element.end());
    if (itr != list.end()) 
        return itr->second->create_func(parent);
    return nullptr;
}


/// <summary>
/// longui namespace
/// </summary>
namespace LongUI {
    // detail namespace
    namespace detail {
        /// <summary>
        /// Marks the wndmin changed.
        /// </summary>
        /// <returns></returns>
        void mark_wndmin_changed() noexcept {
            CUIManager::Private::MarkWndMin();
        }
    }
    // help
    enum {
        pmanag1 = sizeof(CUIManager::Private),
        pm_size = detail::private_manager<sizeof(void*)>::size,
        pm_align= detail::private_manager<sizeof(void*)>::align,
    };
    // check
    static_assert(pm_size == pmanag1, "must be same");
    static_assert(pm_align == alignof(CUIManager::Private), "must be same");
    /// <summary>
    /// single instance buffer for longui manager
    /// </summary>
    std::aligned_storage<sizeof(CUIManager), alignof(CUIManager)>::type s_bufManager;
    // impl
    namespace impl {
        // init HiDPI
        void init_high_dpi_support() noexcept;
        // uninit HiDPI
        void uninit_high_dpi_support() noexcept;
#ifndef NDEBUG
        // create debug window
        auto create_debug_window() noexcept->CUIWindow*;
#endif
        // create control
        UIControl* create_control(const char*a, const char*b, UIControl*p) noexcept {
            return UIManager.CreateControl({ a,b }, p);
        }
        // search for last end
        void search_capsule_for_last_end(UIControl& ctrl) noexcept {
            UIManager.RefreshTimeCapsule(ctrl);
        }

    }
}

/// <summary>
/// Calls the time capsule s1.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::call_time_capsule_s1() noexcept {
    // 获取必要数据
    const auto delta = this_()->m_fDeltaTime;
    const HWND hwnd = UIManager.m_hToolWnd;
    const UINT msg = CallLater::Later_CallTimeCapsule;
    // 解除上级锁
    this->DataUnlock();
    // 发送消息
    const auto wp = reinterpret_cast<WPARAM>(&m_uiTimeCapsuleWaiter);
    union { LPARAM lp; float time; };
    lp = 0; time = delta;
    ::PostMessageW(hwnd, msg, wp, lp);
}

/// <summary>
/// Calls the time capsule s2.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::call_time_capsule_s2() noexcept {
    // 等待执行完毕
    m_uiTimeCapsuleWaiter.Wait();
}

/// <summary>
/// Tries the recreate_device.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::try_recreate() noexcept {
    // 检查Flag
    if (!m_flagRecreate) return;
    m_flagRecreate = false;
    // LCK
    this_()->RenderLock();
    // 重建设备
    auto hr = this->CUIResMgr::recreate_device(this->config);
    // 重建资源
    if (hr) {
        hr = this->CUIResMgr::recreate_resource();
    }
    // 重建窗口
    if (hr) {
        hr = this->CUIWndMgr::recreate_windows();
    }
    // UNL
    this_()->RenderUnlock();
    // TODO: 错误处理对策
#ifndef NDEBUG
    LUIDebug(Hint)
        << "[Recreate Resource]: "
        << hr
        << endl;
#endif // !NDEBUG
    this->ShowError(hr);
}

/// <summary>
/// Initializes the specified configuration.
/// </summary>
/// <param name="config">The configuration.</param>
/// <returns></returns>
auto LongUI::CUIManager::Initialize(IUIConfigure* cfg) noexcept -> Result {
#ifndef NDEBUG
    // 大小端断言
    ::ui_endian_runtime_assert();
#endif
    // 设置高DPI支持
    impl::init_high_dpi_support();
    // 设置DComp支持
    impl::init_dcomp_support();
    // 初始化COM
    {
        const Result hr = { ::CoInitialize(nullptr) };
        if (!hr) return hr;
    }
    // 默认配置
    constexpr auto DEFCFG_SIZE = sizeof(CUIDefaultConfigure);
    static void* s_config_buf[DEFCFG_SIZE / sizeof(void*)];
    static_assert(sizeof(s_config_buf) == sizeof(CUIDefaultConfigure), "Orz");
    // 直接初始化
    const auto defcfg = reinterpret_cast<CUIDefaultConfigure*>(s_config_buf);
    detail::ctor_dtor<CUIDefaultConfigure>::create(defcfg);
    if (!cfg) cfg = defcfg;
    // 构造管理器
    {
        Result hr = { Result::RS_OK };
        detail::ctor_dtor<CUIManager>::create(&UIManager, cfg, hr);
        if (!hr) return hr;
    }
    // 致命BUG处理
    this_()->InitUnExpHandler();
    // 初始化
    Result hr = { Result::RS_OK };
    // 初始化成员
    if (hr) {
        this_()->pm().time_capsules.reserve(32);
        if (!this_()->pm().time_capsules.is_ok())
            hr = { Result::RE_OUTOFMEMORY };
    }
    // 初始化默认字体
    if (hr) {
        hr = this_()->init_default_font(cfg);
    }
    // 获取实例句柄
    auto hInstance = ::GetModuleHandleW(nullptr);
    // 获取文本格式化器
    if (hr) {
        //hr = configure.CreateInterface(LongUI_IID_PV_ARGS(this_()->m_pTextFormatter));
        longui_debug_hr(hr, "Create this_()->m_pTextFormatter faild");
    }
    // 创建工具窗口
    if (hr) {
        // 注册窗口
        WNDCLASSEXW wcex;
        auto code = ::GetClassInfoExW(
            hInstance, Attribute::WindowClassNameT, &wcex
        );
        if (!code) {
            // 处理函数
            auto wndproc = [](
                HWND hwnd, 
                UINT message, 
                WPARAM wParam,
                LPARAM lParam
                ) noexcept ->LRESULT {
                switch (message)
                {
                case WM_INPUT:
                    // XXX: 考虑加锁
                    //CUIDataAutoLocker locker;
                    UIManager.pm().km_input
                        .Update(reinterpret_cast<HRAWINPUT>(lParam));
                    return 1;
#if 0
                case WM_TIMER:
                {
                    const WPARAM high = wParam & ~WPARAM(3);
                    const uint32_t low = wParam & 3;
                    const auto ctrl = reinterpret_cast<UIControl*>(high);
                    CUIDataAutoLocker locker;
                    LongUI::OnTimer(*ctrl, low);
                    return 0;
                }
#endif
                case WM_SETTINGCHANGE:
                    UIManager.refresh_system_info();
                    break;
                case WM_DISPLAYCHANGE:
                    // 显示环境改变
                    UIManager.refresh_display_frequency();
                    UIManager.redirect_screen();
#ifndef NDEBUG
                    LUIDebug(Hint) << "WM_DISPLAYCHANGE" << LongUI::endl;
                case WM_CLOSE:
                    // 不能关闭该窗口
#endif
                    break;
                case CallLater::Later_DeleteControl:
                {
                    // 延迟删除控件
                    CUIDataAutoLocker locker;
                    LongUI::DeleteControl(reinterpret_cast<UIControl*>(wParam));
                    break;
                }
                case CallLater::Later_CallTimeCapsule:
                {
                    // 调用时间胶囊
                    const auto ptr = reinterpret_cast<CUIWaiter*>(wParam);
                    union { LPARAM lp; float time; };
                    lp = lParam;
                    UIManager.update_time_capsule(time);
                    ptr->Broadcast();
                }
                    break;
                default:
                    return ::DefWindowProcW(hwnd, message, wParam, lParam);
                }
                return 0;
            };
            // 注册窗口类
            wcex = { 0 };
            wcex.cbSize = sizeof(WNDCLASSEXW);
            wcex.style = CS_NOCLOSE;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = sizeof(void*);
            wcex.hInstance = hInstance;
            wcex.hCursor = nullptr;
            wcex.hbrBackground = nullptr;
            wcex.lpszMenuName = nullptr;
            wcex.lpszClassName = Attribute::WindowClassNameT;
            wcex.hIcon = nullptr;
            wcex.lpfnWndProc = wndproc;
            ::RegisterClassExW(&wcex);
        }
#ifdef NDEBUG
        constexpr int SIX = 0;
        constexpr int SIY = 0;
        constexpr int SIW = 0;
        constexpr int SIH = 0;
#else
        constexpr int SIX = 50;
        constexpr int SIY = 50;
        constexpr int SIW_ = 256;
        constexpr int SIH_ = 0;
        RECT rect = { SIX, SIY, SIX + SIW_, SIY + SIH_ };
        ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        const int SIW = rect.right - rect.left;
        const int SIH = rect.bottom - rect.top;
#endif
        // 创建
        this_()->m_hToolWnd = ::CreateWindowExW(
            WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
            Attribute::WindowClassNameT, Attribute::WindowClassNameT,
            0, SIX, SIY, SIW, SIH, nullptr, nullptr, hInstance, nullptr
        );
        // 成功
        if (!this_()->m_hToolWnd) hr = { Result::RE_FAIL };
#ifndef NDEBUG
        // 调试
        else if (!(flag & IUIConfigure::Flag_DbgDebugWindow))
            ::ShowWindow(this_()->m_hToolWnd, SW_SHOW);
#endif
    }

    // 初始化输入
    //if (hr) {
    //    hr = this_()->pm().km_input.Init(this_()->m_hToolWnd);
    //}

    // 创建帮助器
    /*if (hr) {
        hr = ::CoCreateInstance(
            CLSID_DragDropHelper,
            nullptr,
            CLSCTX_INPROC_SERVER,
            LongUI_IID_PV_ARGS(this_()->m_pDropTargetHelper)
        );
        longui_debug_hr(hr, "CoCreateInstance CLSID_DragDropHelper faild");
    }*/
    // 创建字体集
    if (hr) {
        // 获取脚本
        /*configure.CreateInterface(LongUI_IID_PV_ARGS(this_()->m_pFontCollection));
        // 失败获取系统字体集
        if (!this_()->m_pFontCollection) {
            hr = this_()->m_pDWriteFactory->GetSystemFontCollection(&this_()->m_pFontCollection);
            longui_debug_hr(hr, "this_()->m_pDWriteFactory->GetSystemFontCollection faild");
        }*/
    }
#ifndef NDEBUG
    // 枚举字体
    /*if (hr && (this_()->flag & IUIConfigure::Flag_DbgOutputFontFamily)) {
        auto count = this_()->m_pFontCollection->GetFontFamilyCount();
        UIManager << DL_Log << "Font found: " << long(count) << "\r\n";
        // 遍历所有字体
        for (auto i = 0u; i < count; ++i) {
            IDWriteFontFamily* family = nullptr;
            // 获取字体信息
            if (SUCCEEDED(this_()->m_pFontCollection->GetFontFamily(i, &family))) {
                IDWriteLocalizedStrings* string = nullptr;
                // 获取字体名称
                if (SUCCEEDED(family->GetFamilyNames(&string))) {
                    wchar_t buffer[LongUIStringBufferLength];
                    auto tc = string->GetCount();
                    UIManager << DLevel_Log << Formated("%4d[%d]: ", int(i), int(tc));
                    // 遍历所有字体名称
#if 0
                    for (auto j = 0u; j < 1u; j++) {
                        string->GetLocaleName(j, buffer, LongUIStringBufferLength);
                        UIManager << DLevel_Log << buffer << " => ";
                        // 有些语言在我的机器上显示不了(比如韩语), 会出现bug略过不少东西, 就显示第一个了
                        string->GetString(j, buffer, LongUIStringBufferLength);
                        UIManager << DLevel_Log << buffer << "; ";
                    }
#else
                    // 显示第一个
                    string->GetLocaleName(0, buffer, LongUIStringBufferLength);
                    UIManager << DLevel_Log << buffer << " => ";
                    string->GetString(0, buffer, LongUIStringBufferLength);
                    UIManager << DLevel_Log << buffer << ";\r\n";
#endif
                }
                LongUI::SafeRelease(string);
            }
            LongUI::SafeRelease(family);
        }
        // 刷新
        UIManager << DL_Log << LongUI::endl;
    }*/
#endif
    // 添加控件
    if (hr) {
        ControlInfoList list;
        // 添加默认控件
        LongUI::AddDefaultControlInfo(list);
        // 添加自定义控件
        if (list) this_()->config->RegisterControl(list);
        // 注册控件
        if (list) for (auto info : list) {
            if (!this_()->pm().cclasses.insert({ info->element_name, info }).second) {
                goto error_oom;
            }
        }
        else {
        error_oom:
            hr = { Result::RE_OUTOFMEMORY };
            assert(!"check this");
        }
    }
    // 初始化事件
    /*if (hr) {
        hr = this_()->do_creating_event(LongUI::CreateEventType::Type_Initialize);
        longui_debug_hr(hr, "do_creating_event(init) faild");
    }
    // 创建资源
    if (hr) {
        hr = this_()->RecreateResources();
        longui_debug_hr(hr, "RecreateResources faild");
    }*/
    // 第一次重建设备资源
    if (hr) {
        auto& pmm = this_()->pm();
        hr = this_()->recreate_device(this_()->config);
    }
    // 检查当前路径
#ifndef NDEBUG
    constexpr uint32_t buflen = MAX_PATH * 4;
    wchar_t buffer[buflen]; buffer[0] = 0;
    ::GetCurrentDirectoryW(buflen, buffer);
    LUIDebug(Hint)
        << " Current Directory: "
        << buffer
        << LongUI::endl;
#endif
    // 创建调试窗口
#ifndef NDEBUG
    if (hr && (flag & IUIConfigure::Flag_DbgDebugWindow)) {
        m_pDebugWindow = impl::create_debug_window();
        if (!m_pDebugWindow) hr = { Result::RE_OUTOFMEMORY };
    }
#endif // !NDEBUG

    return hr;
}

PCN_NOINLINE
/// <summary>
/// Deletes the later.
/// </summary>
/// <param name="">The .</param>
/// <returns></returns>
void LongUI::CUIManager::DeleteLater(UIControl& ctrl) noexcept {
    // 检查删除性
    if (LongUI::CheckControlDeleteLater(ctrl)) return;
    LongUI::MarkControlDeleteLater(ctrl);
    // 进行标记删除
    const HWND hwnd = UIManager.m_hToolWnd;
    const UINT msg = CallLater::Later_DeleteControl;
    const auto wpa = reinterpret_cast<WPARAM>(&ctrl);
    const auto rv = ::PostMessageW(hwnd, msg, wpa, 0); rv;
    assert(rv && "post message failed, maybe too many messages");
}

#ifndef NDEBUG
#include <core/ui_string.h>
#endif

/// <summary>
/// Initializes a new instance of the <see cref="CUIManager"/> class.
/// </summary>
/// <param name="config">The configuration.</param>
LongUI::CUIManager::CUIManager(IUIConfigure* config, Result& out) noexcept :
ConfigKeeper{ config },
#ifndef NDEBUG
CUIDebug(config->GetSimpleLogFileName().c_str()),
#endif
CUIResMgr(config, out),
CUIWndMgr(out),
flag(config->GetConfigureFlag()) {
#ifndef NDEBUG
    detail::ctor_dtor<CUIManagerDebug>::create(&DbgMgr());
#endif
    //config->AddRef();
    detail::ctor_dtor<Private>::create(&this_()->pm());
    // 更新系统信息
    this_()->refresh_system_info();
    
    //out = { Result::RE_FAIL };
}

/// <summary>
/// Finalizes an instance of the <see cref="CUIManager"/> class.
/// </summary>
/// <returns></returns>
LongUI::CUIManager::~CUIManager() noexcept {

    this_()->pm().~Private();
#ifndef NDEBUG
    DbgMgr().~CUIManagerDebug();
#endif
}

/// <summary>
/// Uninitializes this instance.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::Uninitialize() noexcept {
    // 手动调用析构函数
    UIManager.~CUIManager();
    // 反初始化COM
    ::CoUninitialize();
    // 反初始化DComp支持
    impl::uninit_dcomp_support();
    // 反初始化高DPI支持
    impl::uninit_high_dpi_support();
}

/// <summary>
/// Refreshes the system information.
/// </summary>
/// <returns></returns>
void LongUI::CUIManager::refresh_system_info() noexcept {
    UINT data;
    ::SystemParametersInfoA(SPI_GETWHEELSCROLLLINES, 0, &data, 0);
    this_()->m_uWheelScrollLines = data;
    ::SystemParametersInfoA(SPI_GETWHEELSCROLLCHARS, 0, &data, 0);
    this_()->m_uWheelScrollChars = data;
}

PCN_NOINLINE
/// <summary>
/// Afters the create tc.
/// </summary>
/// <param name="tc">The tc.</param>
/// <param name="ctrl">The control.</param>
/// <returns></returns>
auto LongUI::CUIControlControl::after_create_tc(CUITimeCapsule* tc, 
    UIControl* ctrl) noexcept -> CUITimeCapsule* {
    // 有效指针
    if (tc) {
        auto&self = UIManager;
        // 连接前后节点
        self.m_oTailTimeCapsule.prev->next = tc;
        tc->prev = self.m_oTailTimeCapsule.prev;
        tc->next = &self.m_oTailTimeCapsule;
        self.m_oTailTimeCapsule.prev = tc;
        // 设置指针
        if (ctrl) {
            tc->SetHoster(*ctrl);
            self.refresh_time_capsule(*ctrl, *tc);
        }
    }
    return tc;
}


/// <summary>
/// Shows the error.
/// </summary>
/// <param name="hr">The hr.</param>
/// <param name="str">The string.</param>
/// <returns></returns>
bool LongUI::CUIManager::ShowError(Result hr, const wchar_t* str) noexcept {
    assert(hr);
    //this->config->OnError();
    return 0;
}

PCN_NOINLINE
/// <summary>
/// Loads the data from URL.
/// </summary>
/// <param name="url_in_utf8">The URL in UTF8.</param>
/// <param name="buffer">The buffer.</param>
/// <returns></returns>
void LongUI::CUIManager::LoadDataFromUrl(
    U8View url_in_utf8,
    POD::Vector<uint8_t>& buffer) noexcept {
    const auto url_utf16 = CUIString::FromUtf8(url_in_utf8);
    // 读取文件
    if (CUIFile file{ url_utf16.c_str(), CUIFile::Flag_Read }) {
        const auto file_size = file.GetFilezize();
        // +1 针对字符串的优化处理
        buffer.reserve(file_size + 1);
        buffer.resize(file_size);
        if (buffer.is_ok()) file.Read(&buffer.front(), file_size);
        return;
    }
    // 查找失败, 利用config接口读取
    this->config->LoadDataFromUrl(url_in_utf8, url_utf16, buffer);
    // 载入失败
#ifndef NDEBUG
    if (buffer.empty())
        LUIDebug(Error)
            << "load file failed: "
            << url_utf16
            << endl;
#endif // !NDEBUG

}

#ifndef NDEBUG
/// <summary>
/// the endian runtime assert.
/// </summary>
/// <returns></returns>
void ui_endian_runtime_assert() noexcept {
    using namespace LongUI;
    enum : uint32_t {
        LITTLE_ENDIAN = 0x03020100ul,
        BIG_ENDIAN = 0x00010203ul,
        PDP_ENDIAN = 0x01000302ul,
    };
    const union { unsigned char bytes[4]; uint32_t value; }
    host_order{ { 0, 1, 2, 3 } };
    const bool le = host_order.value == LITTLE_ENDIAN;
    const bool be = host_order.value == BIG_ENDIAN;
    const bool me = host_order.value == PDP_ENDIAN;
    assert(le == helper::is_little_endian::value);
    assert(be == helper::is_big_endian::value);
    assert(me == helper::is_pdp_endian::value);
}

#endif


// ----------------------------------------------------------------------------
//                          UI Window Manager
// ----------------------------------------------------------------------------

/// <summary>
/// Exits this instance.
/// </summary>
/// <returns></returns>
void LongUI::CUIWndMgr::exit() noexcept {
    UIManager.Exit();
}

/// <summary>
/// Determines whether [is quit on last window closed].
/// </summary>
/// <returns></returns>
bool LongUI::CUIWndMgr::is_quit_on_last_window_closed() noexcept {
    return !!(UIManager.flag & IUIConfigure::Flag_QuitOnLastWindowClosed);
}
