#include "options.h"
#include <gui/builder.h>
#include "jwin.h"
#include "zq/zquest.h"
#include "tiles.h"
#include "zq/zq_misc.h"
#include "zc_list_data.h"
#include "alert.h"

extern bool reload_fonts;
void load_size_poses();

void call_options_dlg()
{
	OptionsDialog().show();
}

extern int32_t EnableTooltips, GridColor, CmbCursorCol, TilePgCursorCol,
	CmbPgCursorCol, KeyboardRepeatDelay, TooltipsHighlight, KeyboardRepeatRate,
	pixeldb, infobg, MMapCursorStyle;
extern bool allowHideMouse;

void OptionsDialog::loadOptions()
{
	opts[OPT_MOUSESCROLL] = MouseScroll ? 1 : 0;
	opts[OPT_SAVEPATHS] = SavePaths ? 1 : 0;
	opts[OPT_PALCYCLE] = CycleOn ? 1 : 0;
	opts[OPT_VSYNC] = Vsync ? 1 : 0;
	opts[OPT_FPS] = ShowFPS ? 1 : 0;
	opts[OPT_SAVEDRAGRESIZE] = SaveDragResize ? 1 : 0;
	opts[OPT_SAVEWINPOS] = SaveWinPos ? 1 : 0;
	opts[OPT_DRAGASPECT] = DragAspect ? 1 : 0;
	opts[OPT_COMB_BRUSH] = ComboBrush ? 1 : 0;
	opts[OPT_FLOAT_BRUSH] = FloatBrush ? 1 : 0;
	opts[OPT_RELOAD_QUEST] = OpenLastQuest ? 1 : 0;
	opts[OPT_MISALIGNS] = ShowMisalignments ? 1 : 0;
	opts[OPT_ANIM_COMBOS] = AnimationOn ? 1 : 0;
	opts[OPT_OW_PROT] = OverwriteProtection ? 1 : 0;
	opts[OPT_TILE_PROT] = TileProtection ? 1 : 0;
	opts[OPT_STATIC_INVAL] = InvalidStatic ? 1 : 0;
	opts[OPT_RULESET] = RulesetDialog ? 1 : 0;
	opts[OPT_TOOLTIPS] = EnableTooltips ? 1 : 0;
	opts[OPT_TOOLTIP_HIGHLIGHT] = TooltipsHighlight ? 1 : 0;
	opts[OPT_TOOLTIP_TIMER] = tooltip_maxtimer;
	opts[OPT_MAPCURSOR] = MMapCursorStyle;
	opts[OPT_PATTERNSEARCH] = abc_patternmatch ? 1 : 0;
	opts[OPT_NEXTPREVIEW] = NoScreenPreview ? 1 : 0;
	opts[OPT_INITSCR_WARN] = WarnOnInitChanged ? 1 : 0;
	opts[OPT_ABRETENTION] = AutoBackupRetention;
	opts[OPT_ASINTERVAL] = AutoSaveInterval;
	opts[OPT_ASRETENTION] = AutoSaveRetention;
	opts[OPT_UNCOMP_AUTOSAVE] = UncompressedAutoSaves ? 1 : 0;
	opts[OPT_GRIDCOL] = GridColor;
	opts[OPT_CMB_CURS_COL] = CmbCursorCol;
	opts[OPT_TPG_CURS_COL] = TilePgCursorCol;
	opts[OPT_CPG_CURS_COL] = CmbPgCursorCol;
	opts[OPT_SNAPFORMAT] = SnapshotFormat;
	opts[OPT_KBREPDEL] = KeyboardRepeatDelay;
	opts[OPT_KBREPRATE] = KeyboardRepeatRate;
	opts[OPT_CURS_LARGE] = int32_t(zc_get_config("zquest","cursor_scale_large",1.5)*10000);
	opts[OPT_COMPILE_OK] = zc_get_config("Compiler", "compile_success_sample", 20);
	opts[OPT_COMPILE_ERR] = zc_get_config("Compiler", "compile_error_sample", 28);
	opts[OPT_COMPILE_DONE] = zc_get_config("Compiler", "compile_finish_sample", 20);
	opts[OPT_COMPILE_VOL] = zc_get_config("Compiler", "compile_audio_volume", 100);
	opts[OPT_DISABLE_LPAL_SHORTCUT] = DisableLPalShortcuts;
	opts[OPT_DISABLE_COMPILE_CONSOLE] = DisableCompileConsole;
	opts[OPT_SKIP_LAYER_WARNING] = skipLayerWarning;
	opts[OPT_NUMERICAL_FLAG_LIST] = numericalFlags;
	opts[OPT_CUSTOMFONT] = zc_get_config("gui","custom_fonts",1);
	opts[OPT_BOTTOM8] = pixeldb;
	opts[OPT_INFO_BG] = infobg;
	opts[OPT_HIDEMOUSE] = allowHideMouse?1:0;
	opts[OPT_COMPILEONSAVE] = zc_get_config("zquest","quick_compile_on_save",0)?1:0;
	
	int deffont_ids[CFONT_MAX] = {font_lfont_l,font_lfont,font_pfont,font_nfont,font_sfont3,font_lfont,font_lfont_l};
	char const* _font_titles[CFONT_MAX] = {"dialog", "gui", "title", "favcmd", "textbox", "ttip", "info"};
	char const* cpref = "compact";
	char const* lpref = "large";
	char buf[512];
	for(int q = 0; q < CFONT_MAX; ++q)
	{
		sprintf(buf, "font_%s_%s", lpref, _font_titles[q]);
		opts[q+OPT_FIRSTFONT] = zc_get_config("ZQ_GUI", buf, deffont_ids[q]);
		sprintf(buf, "font_%s_%s", cpref, _font_titles[q]);
		opts[q+OPT_FIRSTFONT+CFONT_MAX] = zc_get_config("ZQ_GUI", buf, deffont_ids[q]);
		sprintf(buf, "fontscale_%s_%s", lpref, _font_titles[q]);
		opts[q+OPT_FIRST_FONTSCALE] = zc_get_config("ZQ_GUI", buf, 1);
		sprintf(buf, "fontscale_%s_%s", cpref, _font_titles[q]);
		opts[q+OPT_FIRST_FONTSCALE+CFONT_MAX] = zc_get_config("ZQ_GUI", buf, 1);
	}
	
	//cleanup
    reset_combo_animations();
    reset_combo_animations2();
    go();
}

void OptionsDialog::saveOptions()
{
	for(auto ind = 0; ind < OPT_MAX; ++ind)
	{
		if(opt_changed[ind])
			saveOption(ind);
	}
	if(opt_changed[OPT_CUSTOMFONT])
		reload_fonts = true;
	else
	{
		for(int ind = OPT_FIRSTFONT; ind <= OPT_LAST_FONTSCALE; ++ind)
		{
			if(opt_changed[ind])
			{
				reload_fonts = true;
				break;
			}
		}
	}
	set_keyboard_rate(KeyboardRepeatDelay,KeyboardRepeatRate); //Reset keyboard rate
	load_mice(); //Reset cursor scale
}
void OptionsDialog::saveOption(int ind)
{
	auto v = opts[ind];
	switch(ind)
	{
		case OPT_SKIP_LAYER_WARNING:
			skipLayerWarning = v;
			zc_set_config("zquest", "skip_layer_warning", v);
			break;
		case OPT_MOUSESCROLL:
			MouseScroll = v;
			zc_set_config("zquest", "mouse_scroll", v);
			break;
		case OPT_DISABLE_LPAL_SHORTCUT:
			DisableLPalShortcuts = v;
			zc_set_config("zquest", "dis_lpal_shortcut", v);
			break;
		case OPT_DISABLE_COMPILE_CONSOLE:
			DisableCompileConsole = v;
			zc_set_config("zquest", "internal_compile_console", v);
			break;
		case OPT_INITSCR_WARN:
			WarnOnInitChanged = v;
			zc_set_config("zquest", "warn_initscript_changes", v);
			break;
		case OPT_STATIC_INVAL:
			InvalidStatic = v;
			zc_set_config("zquest", "invalid_static", v);
			break;
		case OPT_NUMERICAL_FLAG_LIST:
			numericalFlags = v;
			zc_set_config("zquest", "numerical_flags", v);
			break;
		case OPT_TILE_PROT:
			TileProtection = v;
			zc_set_config("zquest", "tile_protection", v);
			break;
		case OPT_GRIDCOL:
			GridColor = v;
			zc_set_config("zquest", "grid_color", v);
			break;
		case OPT_CMB_CURS_COL:
			CmbCursorCol = v;
			zc_set_config("zquest", "combo_cursor_color", v);
			break;
		case OPT_TPG_CURS_COL:
			TilePgCursorCol = v;
			zc_set_config("zquest", "tpage_cursor_color", v);
			break;
		case OPT_CPG_CURS_COL:
			CmbPgCursorCol = v;
			zc_set_config("zquest", "cpage_cursor_color", v);
			break;
		case OPT_SNAPFORMAT:
			SnapshotFormat = v;
			zc_set_config("zquest", "snapshot_format", v);
			break;
		case OPT_RELOAD_QUEST:
			OpenLastQuest = v;
			zc_set_config("zquest", "open_last_quest", v);
			break;
		case OPT_MISALIGNS:
			ShowMisalignments = v;
			zc_set_config("zquest", "show_misalignments", v);
			break;
		case OPT_CURS_LARGE:
			zc_set_config("zquest", "cursor_scale_large", v / 10000.0);
			break;
		case OPT_COMPILE_OK:
			zc_set_config("Compiler", "compile_success_sample", v);
			break;
		case OPT_COMPILE_ERR:
			zc_set_config("Compiler", "compile_error_sample", v);
			break;
		case OPT_COMPILE_DONE:
			zc_set_config("Compiler", "compile_finish_sample", v);
			break;
		case OPT_COMPILE_VOL:
			zc_set_config("Compiler", "compile_audio_volume", v);
			break;
		case OPT_NEXTPREVIEW:
			NoScreenPreview = v;
			zc_set_config("zquest","no_preview",v);
			break;
		case OPT_PATTERNSEARCH:
			abc_patternmatch = v;
			zc_set_config("zquest","lister_pattern_matching",v);
			break;
		case OPT_TOOLTIPS:
			EnableTooltips = v;
			zc_set_config("zquest","enable_tooltips",v);
			break;
		case OPT_TOOLTIP_HIGHLIGHT:
			TooltipsHighlight = v;
			zc_set_config("zquest","ttip_highlight",v);
			break;
		case OPT_TOOLTIP_TIMER:
			tooltip_maxtimer = v;
			zc_set_config("zquest","ttip_timer",v);
			break;
		case OPT_MAPCURSOR:
			MMapCursorStyle = v;
			zc_set_config("zquest","cursorblink_style",v);
			break;
		case OPT_RULESET:
			RulesetDialog = v;
			zc_set_config("zquest","rulesetdialog",v);
			break;
		case OPT_KBREPDEL:
			KeyboardRepeatDelay = v;
			zc_set_config("zquest","keyboard_repeat_delay",v);
			break;
		case OPT_KBREPRATE:
			KeyboardRepeatRate = v;
			zc_set_config("zquest","keyboard_repeat_rate",v);
			break;
		case OPT_ANIM_COMBOS:
			AnimationOn = v;
			zc_set_config("zquest","animation_on",v);
			break;
		case OPT_ABRETENTION:
			AutoBackupRetention = v;
			zc_set_config("zquest","auto_backup_retention",v);
			break;
		case OPT_ASINTERVAL:
			time(&auto_save_time_start); //Reset autosave timer
			AutoSaveInterval = v;
			zc_set_config("zquest","auto_save_interval",v);
			break;
		case OPT_ASRETENTION:
			AutoSaveRetention = v;
			zc_set_config("zquest","auto_save_retention",v);
			break;
		case OPT_UNCOMP_AUTOSAVE:
			UncompressedAutoSaves = v;
			zc_set_config("zquest","uncompressed_auto_saves",v);
			break;
		case OPT_OW_PROT:
			OverwriteProtection = v;
			zc_set_config("zquest","overwrite_prevention",v);
			break;
		case OPT_SAVEPATHS:
			SavePaths = v;
			zc_set_config("zquest","save_paths",v);
			break;
		case OPT_PALCYCLE:
			CycleOn = v;
			zc_set_config("zquest","cycle_on",v);
			break;
		case OPT_VSYNC:
			Vsync = v;
			zc_set_config("zquest","vsync",v);
			break;
		case OPT_FPS:
			ShowFPS = v;
			zc_set_config("zquest","showfps",v);
			break;
		case OPT_SAVEDRAGRESIZE:
			SaveDragResize = v;
			zc_set_config("zquest","save_drag_resize",v);
			break;
		case OPT_DRAGASPECT:
			DragAspect = v;
			zc_set_config("zquest","drag_aspect",v);
			break;
		case OPT_SAVEWINPOS:
			SaveWinPos = v;
			zc_set_config("zquest","save_window_position",v);
			break;
		case OPT_COMB_BRUSH:
			ComboBrush = v;
			zc_set_config("zquest","combo_brush",v);
			break;
		case OPT_FLOAT_BRUSH:
			FloatBrush = v;
			zc_set_config("zquest","float_brush",v);
			break;
		case OPT_CUSTOMFONT:
			zc_set_config("gui","custom_fonts",v);
			break;
		case OPT_BOTTOM8:
			pixeldb = v;
			zc_set_config("ZQ_GUI","bottom_8_pixels",v);
			break;
		case OPT_INFO_BG:
			infobg = v;
			zc_set_config("ZQ_GUI","info_text_bg",v);
			break;
		case OPT_HIDEMOUSE:
			allowHideMouse = v!=0;
			zc_set_config("ZQ_GUI","allowHideMouse",v);
			break;
		case OPT_COMPILEONSAVE:
			zc_set_config("zquest","quick_compile_on_save",v);
			break;
		
		case OPT_LARGEFONT_DIALOG:
			zc_set_config("ZQ_GUI", "font_large_dialog", v);
			break;
		case OPT_LARGEFONT_GUI:
			zc_set_config("ZQ_GUI", "font_large_gui", v);
			break;
		case OPT_LARGEFONT_TITLE:
			zc_set_config("ZQ_GUI", "font_large_title", v);
			break;
		case OPT_LARGEFONT_FAVCMD:
			zc_set_config("ZQ_GUI", "font_large_favcmd", v);
			break;
		case OPT_LARGEFONT_TEXTBOX:
			zc_set_config("ZQ_GUI", "font_large_textbox", v);
			break;
		case OPT_LARGEFONT_TTIP:
			zc_set_config("ZQ_GUI", "font_large_ttip", v);
			break;
		case OPT_LARGEFONT_INFO:
			zc_set_config("ZQ_GUI", "font_large_info", v);
			break;
		
		case OPT_COMPACTFONT_DIALOG:
			zc_set_config("ZQ_GUI", "font_compact_dialog", v);
			break;
		case OPT_COMPACTFONT_GUI:
			zc_set_config("ZQ_GUI", "font_compact_gui", v);
			break;
		case OPT_COMPACTFONT_TITLE:
			zc_set_config("ZQ_GUI", "font_compact_title", v);
			break;
		case OPT_COMPACTFONT_FAVCMD:
			zc_set_config("ZQ_GUI", "font_compact_favcmd", v);
			break;
		case OPT_COMPACTFONT_TEXTBOX:
			zc_set_config("ZQ_GUI", "font_compact_textbox", v);
			break;
		case OPT_COMPACTFONT_TTIP:
			zc_set_config("ZQ_GUI", "font_compact_ttip", v);
			break;
		case OPT_COMPACTFONT_INFO:
			zc_set_config("ZQ_GUI", "font_compact_info", v);
			break;
		
		case OPT_LARGEFONT_SCALE_DIALOG:
			zc_set_config("ZQ_GUI", "fontscale_large_dialog", v);
			break;
		case OPT_LARGEFONT_SCALE_GUI:
			zc_set_config("ZQ_GUI", "fontscale_large_gui", v);
			break;
		case OPT_LARGEFONT_SCALE_TITLE:
			zc_set_config("ZQ_GUI", "fontscale_large_title", v);
			break;
		case OPT_LARGEFONT_SCALE_FAVCMD:
			zc_set_config("ZQ_GUI", "fontscale_large_favcmd", v);
			break;
		case OPT_LARGEFONT_SCALE_TEXTBOX:
			zc_set_config("ZQ_GUI", "fontscale_large_textbox", v);
			break;
		case OPT_LARGEFONT_SCALE_TTIP:
			zc_set_config("ZQ_GUI", "fontscale_large_ttip", v);
			break;
		case OPT_LARGEFONT_SCALE_INFO:
			zc_set_config("ZQ_GUI", "fontscale_large_info", v);
			break;
		
		case OPT_COMPACTFONT_SCALE_DIALOG:
			zc_set_config("ZQ_GUI", "fontscale_compact_dialog", v);
			break;
		case OPT_COMPACTFONT_SCALE_GUI:
			zc_set_config("ZQ_GUI", "fontscale_compact_gui", v);
			break;
		case OPT_COMPACTFONT_SCALE_TITLE:
			zc_set_config("ZQ_GUI", "fontscale_compact_title", v);
			break;
		case OPT_COMPACTFONT_SCALE_FAVCMD:
			zc_set_config("ZQ_GUI", "fontscale_compact_favcmd", v);
			break;
		case OPT_COMPACTFONT_SCALE_TEXTBOX:
			zc_set_config("ZQ_GUI", "fontscale_compact_textbox", v);
			break;
		case OPT_COMPACTFONT_SCALE_TTIP:
			zc_set_config("ZQ_GUI", "fontscale_compact_ttip", v);
			break;
		case OPT_COMPACTFONT_SCALE_INFO:
			zc_set_config("ZQ_GUI", "fontscale_compact_info", v);
			break;
	}
}

OptionsDialog::OptionsDialog() : sfx_list(GUI::ZCListData::sfxnames(true))
{
	loadOptions();
	memset(opt_changed, 0, sizeof(opt_changed));
}

//{ Macros
#define OPT_CHECK(optind, optlabel) \
Checkbox( \
	checked = opts[optind], \
	text = optlabel, \
	hAlign = 0.0, \
	onToggleFunc = [&](bool state) \
	{ \
		opts[optind] = state ? 1 : 0; \
		opt_changed[optind] = true; \
	} \
)

#define ROW_CHECK(optind, optlabel) \
Checkbox( \
	colSpan = 2, \
	checked = opts[optind], \
	text = optlabel, \
	hAlign = 0.0, \
	vPadding = DEFAULT_PADDING/2, \
	onToggleFunc = [&](bool state) \
	{ \
		opts[optind] = state ? 1 : 0; \
		opt_changed[optind] = true; \
	} \
), \
Button(forceFitH = true, text = "?", padding = 0_px, disabled = true)

#define ROW_CHECK_I(optind, optlabel, info) \
Checkbox( \
	colSpan = 2, \
	checked = opts[optind], \
	text = optlabel, \
	hAlign = 0.0, \
	vPadding = DEFAULT_PADDING/2, \
	onToggleFunc = [&](bool state) \
	{ \
		opts[optind] = state ? 1 : 0; \
		opt_changed[optind] = true; \
	} \
), \
Button(forceFitH = true, text = "?", \
	disabled = !info[0], padding = 0_px, \
	onPressFunc = [&]() \
	{ \
		InfoDialog("Info",info).show(); \
	})

#define ROW_TF_RANGE(optind, optlabel, minval, maxval) \
Label(text = optlabel, hAlign = 0.0), \
TextField(type = GUI::TextField::type::INT_DECIMAL, \
	fitParent = true, \
	hAlign = 1.0, low = minval, high = maxval, val = opts[optind], \
	minwidth = 4.5_em, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	}), \
DummyWidget()

#define ROW_TF_RANGE_I(optind, optlabel, minval, maxval, info) \
Label(text = optlabel, hAlign = 0.0), \
TextField(type = GUI::TextField::type::INT_DECIMAL, \
	fitParent = true, \
	hAlign = 1.0, low = minval, high = maxval, val = opts[optind], \
	minwidth = 4.5_em, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	}), \
Button(forceFitH = true, text = "?", \
	disabled = !info[0], \
	onPressFunc = [&]() \
	{ \
		InfoDialog("Info",info).show(); \
	})

#define ROW_TF_FLOAT(optind, optlabel, minval, maxval) \
Label(text = optlabel, hAlign = 0.0), \
TextField(type = GUI::TextField::type::FIXED_DECIMAL, places = 4, \
	fitParent = true, maxLength = 8, \
	hAlign = 1.0, low = minval*10000, high = maxval*10000, val = opts[optind], \
	minwidth = 4.5_em, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	}), \
DummyWidget()

#define ROW_TF_FLOAT_I(optind, optlabel, minval, maxval, info) \
Label(text = optlabel, hAlign = 0.0), \
TextField(type = GUI::TextField::type::FIXED_DECIMAL, places = 4, \
	fitParent = true, maxLength = 8, \
	hAlign = 1.0, low = minval*10000, high = maxval*10000, val = opts[optind], \
	minwidth = 4.5_em, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	}), \
Button(forceFitH = true, text = "?", \
	disabled = !info[0], \
	onPressFunc = [&]() \
	{ \
		InfoDialog("Info",info).show(); \
	})

#define ROW_DDOWN(optind, optlabel, lister) \
Label(text = optlabel, hAlign = 0.0), \
DropDownList( \
	fitParent = true, \
	maxwidth = 14_em, \
	data = lister, \
	selectedValue = opts[optind], \
	onSelectFunc = [&](int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	} \
), \
DummyWidget()

#define ROW_DDOWN_I(optind, optlabel, lister, info) \
Label(text = optlabel, hAlign = 0.0), \
DropDownList( \
	fitParent = true, \
	maxwidth = 14_em, \
	data = lister, \
	selectedValue = opts[optind], \
	onSelectFunc = [&](int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
	} \
), \
Button(forceFitH = true, text = "?", \
	disabled = !info[0], \
	onPressFunc = [&]() \
	{ \
		InfoDialog("Info",info).show(); \
	})

#define FONT_ROW_DDOWN(optind, scaleind, optlabel, lister, maxscale) \
Label(text = optlabel, hAlign = 0.0), \
DropDownList( \
	fitParent = true, \
	maxwidth = 14_em, \
	data = lister, \
	selectedValue = opts[optind], \
	onSelectFunc = [&](int32_t val) \
	{ \
		opts[optind] = val; \
		opt_changed[optind] = true; \
		preview_font(val, opts[scaleind]); \
	} \
), \
Button(text = "Prev", \
	forceFitH = true, \
	onPressFunc = [&]() \
	{ \
		preview_font(opts[optind], opts[scaleind]); \
	}), \
TextField(type = GUI::TextField::type::INT_DECIMAL, \
	fitParent = true, \
	hAlign = 1.0, low = 1, high = maxscale, val = opts[scaleind], \
	minwidth = 4.5_em, \
	onValChangedFunc = [&](GUI::TextField::type,std::string_view,int32_t val) \
	{ \
		preview_font(opts[optind], val); \
		opts[scaleind] = val; \
		opt_changed[scaleind] = true; \
	})

//}

//{ Listers
static const GUI::ListData abRetentionList
{
	{ "Disabled", 0 },
	{ " 1", 1 },
	{ " 2", 2 },
	{ " 3", 3 },
	{ " 4", 4 },
	{ " 5", 5 },
	{ " 6", 6 },
	{ " 7", 7 },
	{ " 8", 8 },
	{ " 9", 9 },
	{ "10", 10 }
};
static const GUI::ListData asIntervalList
{
	{ "Disabled", 0 },
	{ " 1 Minute", 1 },
	{ " 2 Minutes", 2 },
	{ " 3 Minutes", 3 },
	{ " 4 Minutes", 4 },
	{ " 5 Minutes", 5 },
	{ " 6 Minutes", 6 },
	{ " 7 Minutes", 7 },
	{ " 8 Minutes", 8 },
	{ " 9 Minutes", 9 },
	{ "10 Minutes", 10 }
};
static const GUI::ListData asRetentionList
{
	{ " 1", 0 },
	{ " 2", 1 },
	{ " 3", 2 },
	{ " 4", 3 },
	{ " 5", 4 },
	{ " 6", 5 },
	{ " 7", 6 },
	{ " 8", 7 },
	{ " 9", 8 },
	{ "10", 9 }
};
static const GUI::ListData colorList
{
	{ "Black", 0 },
	{ "Blue", 1 },
	{ "Green", 2 },
	{ "Cyan", 3 },
	{ "Red", 4 },
	{ "Magenta", 5 },
	{ "Brown", 6 },
	{ "Light Gray", 7 },
	{ "Dark Gray", 8 },
	{ "Light Blue", 9 },
	{ "Light Green", 10 },
	{ "Light Cyan", 11 },
	{ "Light Red", 12 },
	{ "Light Magenta", 13 },
	{ "Yellow", 14 },
	{ "White", 15 }
};
static const GUI::ListData mmapCursList
{
	{ "White", 0 },
	{ "Black+White", 1 },
	{ "Red+Blue", 2 }
};
static const GUI::ListData snapFormatList
{
	{ "BMP", 0 },
	{ "GIF", 1 },
	{ "JPG", 2 },
	{ "PNG", 3 },
	{ "PCX", 4 },
	{ "TGA", 5 }
};
static const GUI::ListData bottom8_list
{
	{ "No Cover", 0 },
	{ "Pixelated Cover", 1 },
	{ "Normal Cover", 2 }
};

//}
FONT* scale_font(FONT* f, int scale); //fonts.cpp
static bool do_prevscale = true;
static int prevfont = 0, prevscale = 1;
void OptionsDialog::preview_font()
{
	static FONT* tempfont = nullptr;
	int scale = do_prevscale ? prevscale : 1;
	if(tempfont)
		destroy_font(tempfont);
	tempfont = scale_font(get_zc_font(prevfont), scale);
	fprev->setFont(tempfont);
	char buf[512];
	sprintf(buf, "Font Preview: %s (%d) [x%d]", get_zc_fontname(prevfont), prevfont, scale);
	fprev_lab->setText(buf);
}
void OptionsDialog::preview_font(int fontind, int scale)
{
	prevfont = fontind;
	prevscale = scale;
	preview_font();
}

size_t tabpos1 = 0, tabpos2 = 0, tabpos3 = 0, font_tab_ptr = 9999;
std::shared_ptr<GUI::Widget> OptionsDialog::view()
{
	using namespace GUI::Builder;
	using namespace GUI::Props;
	
	if(font_tab_ptr > 1000)
		font_tab_ptr = is_compact ? 1 : 0;
	
	static GUI::ListData fontlist;
	fontlist = GUI::ZCListData::fonts(false,true,true).filter(
		[](GUI::ListItem& itm)
		{
			return !isBrokenFont(itm.value);
		}
	);
	std::shared_ptr<GUI::TabRef> fontstab;
	std::shared_ptr<GUI::TabRef> togglestab;
	std::shared_ptr<GUI::TabRef> settingstab;
	
	fontstab = TabRef(name = "Fonts",
		Row(
			Column(
				TabPanel(ptr = &font_tab_ptr, fitParent = true,
					TabRef(name = "Expanded", Rows<4>(
						DummyWidget(colSpan=3),
						Row(
							Label(text = "Scale:"),
							INFOBTN("Each row has a different max scale, most are 2 or 3 max. Textbox is 5 max."
								"\nWarning: Take care with making the 'Dialog' or 'GUI' fonts too large, as this may cause"
								" the options dialog to become unusable, requiring you to manually edit 'zquest.cfg' to fix.")
						),
						FONT_ROW_DDOWN(OPT_LARGEFONT_DIALOG, OPT_LARGEFONT_SCALE_DIALOG, "Dialog Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_LARGEFONT_GUI, OPT_LARGEFONT_SCALE_GUI, "GUI Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_LARGEFONT_TITLE, OPT_LARGEFONT_SCALE_TITLE, "Title Font:", fontlist, 3),
						FONT_ROW_DDOWN(OPT_LARGEFONT_FAVCMD, OPT_LARGEFONT_SCALE_FAVCMD, "FavCMD Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_LARGEFONT_TEXTBOX, OPT_LARGEFONT_SCALE_TEXTBOX, "Textbox Font:", fontlist, 5),
						FONT_ROW_DDOWN(OPT_LARGEFONT_TTIP, OPT_LARGEFONT_SCALE_TTIP, "Tooltip Font:", fontlist, 3),
						FONT_ROW_DDOWN(OPT_LARGEFONT_INFO, OPT_LARGEFONT_SCALE_INFO, "Info Font:", fontlist, 3)
					)),
					TabRef(name = "Compact", Rows<4>(
						DummyWidget(colSpan=3),
						Row(
							Label(text = "Scale:"),
							INFOBTN("Each row has a different max scale, most are 2 or 3 max. Textbox is 5 max."
								"\nWarning: Take care with making the 'Dialog' or 'GUI' fonts too large, as this may cause"
								" the options dialog to become unusable, requiring you to manually edit 'zquest.cfg' to fix.")
						),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_DIALOG, OPT_COMPACTFONT_SCALE_DIALOG, "Dialog Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_GUI, OPT_COMPACTFONT_SCALE_GUI, "GUI Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_TITLE, OPT_COMPACTFONT_SCALE_TITLE, "Title Font:", fontlist, 3),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_FAVCMD, OPT_COMPACTFONT_SCALE_FAVCMD, "FavCMD Font:", fontlist, 2),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_TEXTBOX, OPT_COMPACTFONT_SCALE_TEXTBOX, "Textbox Font:", fontlist, 5),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_TTIP, OPT_COMPACTFONT_SCALE_TTIP, "Tooltip Font:", fontlist, 3),
						FONT_ROW_DDOWN(OPT_COMPACTFONT_INFO, OPT_COMPACTFONT_SCALE_INFO, "Info Font:", fontlist, 3)
					))
				),
				Row(
					Button(text = "Default",
						onClick = message::RELOAD,
						vAlign = 0.0,
						onPressFunc = [&]()
						{
							bool doclear = false;
							AlertDialog("Default Fonts",
								"Reset all font dropdowns to defaults?",
								[&](bool ret,bool)
								{
									doclear = ret;
								}).show();
							if(!doclear) return;
							
							opts[OPT_LARGEFONT_DIALOG] = font_lfont_l;
							opts[OPT_LARGEFONT_TITLE] = font_lfont;
							opts[OPT_LARGEFONT_FAVCMD] = font_pfont;
							opts[OPT_LARGEFONT_GUI] = font_nfont;
							opts[OPT_LARGEFONT_TEXTBOX] = font_sfont3;
							opts[OPT_LARGEFONT_TTIP] = font_lfont;
							opts[OPT_LARGEFONT_INFO] = font_lfont_l;
							
							opts[OPT_COMPACTFONT_DIALOG] = font_lfont_l;
							opts[OPT_COMPACTFONT_TITLE] = font_lfont;
							opts[OPT_COMPACTFONT_FAVCMD] = font_pfont;
							opts[OPT_COMPACTFONT_GUI] = font_nfont;
							opts[OPT_COMPACTFONT_TEXTBOX] = font_sfont3;
							opts[OPT_COMPACTFONT_TTIP] = font_lfont;
							opts[OPT_COMPACTFONT_INFO] = font_lfont_l;
							
							for(int q = OPT_FIRSTFONT; q <= OPT_LAST_FONTSCALE; ++q)
							{
								if(q >= OPT_FIRST_FONTSCALE)
									opts[q] = 1;
								opt_changed[q] = true;
							}
							
							preview_font(opts[is_compact ? OPT_COMPACTFONT_DIALOG : OPT_LARGEFONT_DIALOG], 1);
						}),
					Checkbox(checked = do_prevscale,
						text = "Preview Scale",
						hAlign = 0.0,
						onToggleFunc = [&](bool state)
						{
							do_prevscale = !do_prevscale;
							preview_font();
						})
				),
				Row(
					topPadding = 0.5_em,
					vAlign = 1.0,
					spacing = 2_em,
					Button(
						text = "OK",
						minwidth = 90_px,
						onClick = message::OK),
					Button(
						text = "Cancel",
						minwidth = 90_px,
						onClick = message::CANCEL)
				)
			),
			Column(
				fprev_lab = Label(text = "Font Preview", hAlign = 0.0, fitParent = true),
				fprev = Label(text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,"
					" sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim"
					" ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip"
					" ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate"
					" velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat"
					" cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id"
					" est laborum. Duis nec venenatis ipsum. Aenean sed malesuada velit. Vivamus"
					" eget cursus lacus. Pellentesque pharetra dui eget consequat posuere."
					" Quisque vel semper enim."
					"\n\n! @ # $ % ^ && * ( ) [ ] { } + - = | \\ < > , : ; \" ' ~",
					framed = true, padding = 5_px,
					maxLines = 30, height = 30_em,
					width = 300_px, maxwidth = 300_px, fitParent = true)
			)
		)
	);
	togglestab = TabRef(name = "Toggles",
		TabPanel(ptr = &tabpos2,
			TabRef(name = "1", Row(padding = 0_px,
				Rows<3>(vAlign = 0.0,
					ROW_CHECK(OPT_SAVEPATHS, "Save Paths"),
					ROW_CHECK(OPT_PALCYCLE, "Palette Cycle"),
					ROW_CHECK(OPT_VSYNC, "VSync"),
					ROW_CHECK(OPT_FPS, "Show FPS"),
					ROW_CHECK(OPT_RELOAD_QUEST, "Reload Last Quest"),
					ROW_CHECK(OPT_ANIM_COMBOS, "Animate Combos"),
					ROW_CHECK(OPT_OW_PROT, "Overwrite Protection"),
					ROW_CHECK(OPT_TILE_PROT, "Tile Protection"),
					ROW_CHECK(OPT_STATIC_INVAL, "Use Static for Invalid Data"),
					ROW_CHECK(OPT_RULESET, "Show Ruleset Dialog on New Quest"),
					ROW_CHECK(OPT_PATTERNSEARCH, "Listers Use Pattern-Matching Search"),
					ROW_CHECK(OPT_CUSTOMFONT, "Custom Fonts"),
					ROW_CHECK(OPT_TOOLTIPS, "Enable Tooltips"),
					ROW_CHECK(OPT_TOOLTIP_HIGHLIGHT, "Tooltips Highlight Target"),
					ROW_CHECK(OPT_NEXTPREVIEW, "No Next-Screen Preview"),
					ROW_CHECK(OPT_INITSCR_WARN, "Warn on ~Init Script Update"),
					ROW_CHECK(OPT_DISABLE_LPAL_SHORTCUT, "Disable Level Palette Shortcuts"),
					ROW_CHECK(OPT_DISABLE_COMPILE_CONSOLE, "Internal Compile Window")
				),
				Rows<3>(vAlign = 0.0,
					ROW_CHECK(OPT_SKIP_LAYER_WARNING, "Skip Wrong Layer Flag Warning"),
					ROW_CHECK(OPT_NUMERICAL_FLAG_LIST, "Sort Flag List by Flag Number"),
					ROW_CHECK(OPT_SAVEDRAGRESIZE, "Autosave Window Size Changes"),
					ROW_CHECK(OPT_DRAGASPECT, "Lock Aspect Ratio"),
					ROW_CHECK(OPT_SAVEWINPOS, "Autosave Window Position"),
					ROW_CHECK(OPT_MOUSESCROLL, "Mouse Scroll"),
					ROW_CHECK_I(OPT_COMB_BRUSH, "Combo Brush", "Show the combo you would place if you clicked"),
					ROW_CHECK_I(OPT_FLOAT_BRUSH, "Floating Brush", "Make the Combo Brush float above the screen instead of aligning flatly"),
					ROW_CHECK_I(OPT_MISALIGNS, "Show Misaligns", "Show arrows at the edge of the screen to indicate solidity misalignments"),
					ROW_CHECK_I(OPT_INFO_BG, "Show BG behind infotext", "Show black behind the top-left info text"),
					ROW_CHECK_I(OPT_HIDEMOUSE, "Allow Hiding Mouse", "When displaying the combo or alias brush, allow the normal cursor to be hidden."),
					ROW_CHECK_I(OPT_COMPILEONSAVE, "Compile ZScript on Save", "Perform a 'quick compile', recompiling existing scripts and assigning to slots, when saving the quest.")
				)
			))
		)
	);
	settingstab = TabRef(name = "Settings",
		TabPanel(ptr = &tabpos3,
			TabRef(name = "1", Row(padding = 0_px,
				Rows<3>(vAlign = 0.0,
					ROW_DDOWN(OPT_ABRETENTION, "Auto-backup Retention:", abRetentionList),
					ROW_DDOWN(OPT_ASINTERVAL, "Auto-save Interval:", asIntervalList),
					ROW_DDOWN(OPT_ASRETENTION, "Auto-save Retention:", asRetentionList),
					ROW_CHECK(OPT_UNCOMP_AUTOSAVE, "Uncompressed Auto Saves"),
					ROW_DDOWN(OPT_GRIDCOL, "Grid Color:", colorList),
					ROW_DDOWN(OPT_CMB_CURS_COL, "Combo Column SelColor:", colorList),
					ROW_DDOWN(OPT_TPG_CURS_COL, "Tile Page SelColor:", colorList),
					ROW_DDOWN(OPT_CPG_CURS_COL, "Combo Page SelColor:", colorList),
					ROW_DDOWN_I(OPT_MAPCURSOR, "Minimap Cursor:", mmapCursList,
						"The color of the current screen outline on the minimap."
						" Either solid or blinking between two colors."),
					ROW_DDOWN(OPT_SNAPFORMAT, "Snapshot Format:", snapFormatList),
					ROW_TF_RANGE(OPT_KBREPDEL, "Keyboard Repeat Delay:", 0, 99999),
					ROW_TF_RANGE(OPT_KBREPRATE, "Keyboard Repeat Rate:", 0, 99999),
					ROW_TF_FLOAT(OPT_CURS_LARGE, "Cursor Scale", 1, 5)
				),
				Rows<3>(vAlign = 0.0,
					ROW_DDOWN(OPT_COMPILE_OK, "Compile SFX (OK):", sfx_list),
					ROW_DDOWN(OPT_COMPILE_ERR, "Compile SFX (Fail):", sfx_list),
					ROW_DDOWN(OPT_COMPILE_DONE, "Compile SFX (Slots):", sfx_list),
					ROW_TF_RANGE(OPT_COMPILE_VOL, "Compile SFX Volume %:", 0, 500),
					ROW_DDOWN(OPT_BOTTOM8, "Bottom 8 pixels:", bottom8_list),
					ROW_TF_RANGE(OPT_TOOLTIP_TIMER, "Tooltip Timer:", 15, 60*60)
				)
			))
		)
	);
	
	std::shared_ptr<GUI::Window> window = Window(
		title = "ZQuest Options",
		onClose = message::CANCEL,
		Column(
			TabPanel(ptr = &tabpos1,
				minwidth = 360_px,
				
				fontstab,
				togglestab,
				settingstab
			),
			Row(
				topPadding = 0.5_em,
				vAlign = 1.0,
				spacing = 2_em,
				Button(
					text = "OK",
					minwidth = 90_px,
					onClick = message::OK),
				Button(
					text = "Cancel",
					minwidth = 90_px,
					onClick = message::CANCEL)
			)
		)
	);
	
	auto cur_font = opts[is_compact ? OPT_COMPACTFONT_DIALOG : OPT_LARGEFONT_DIALOG];
	auto cur_scale = opts[is_compact ? OPT_COMPACTFONT_SCALE_DIALOG : OPT_LARGEFONT_SCALE_DIALOG];
	preview_font(cur_font, cur_scale);
	return window;
}

bool OptionsDialog::handleMessage(const GUI::DialogMessage<message>& msg)
{
	switch(msg.message)
	{
		case message::OK:
			saveOptions();
			[[fallthrough]];
		case message::CANCEL:
			//cleanup
			setup_combo_animations();
			setup_combo_animations2();
			refresh(rALL);
			comeback();
			return true;
		case message::RELOAD:
			rerun_dlg = true;
			return true;
	}
	return false;
}
