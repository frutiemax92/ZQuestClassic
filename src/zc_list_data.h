#ifndef ZC_LIST_DATA_H
#define ZC_LIST_DATA_H

#include "gui/list_data.h"

namespace GUI::ZCListData
{
  GUI::ListData fonts(bool ss_fonts, bool numbered = true, bool sorted = true);
  GUI::ListData shadow_types();
	GUI::ListData strings(bool combostr = false, bool respect_order = true, bool numbered = true);
  GUI::ListData ss_counters(bool numbered = false, bool skipNone = false);
	GUI::ListData enemies(bool numbered = false, bool defaultFilter = true);
	GUI::ListData items(bool numbered = false, bool none = true);
	GUI::ListData dropsets(bool numbered = false, bool none = false);
  GUI::ListData itemclass(bool numbered = false, bool zero_none = false);
	GUI::ListData combotype(bool numbered = false, bool skipNone = false);
	GUI::ListData mapflag(int32_t numericalFlags, bool numbered = false, bool skipNone = false);
	GUI::ListData counters(bool numbered = false, bool skipNone = false);
	GUI::ListData miscsprites(bool skipNone = true, bool inclNegSpecialVals = false, bool numbered = true);
	GUI::ListData bottletype();
	GUI::ListData dmaps(bool numbered = false);
	GUI::ListData lweaptypes();
	GUI::ListData weaptypes(bool numbered = false);
	GUI::ListData sfxnames(bool numbered = false);
	GUI::ListData midinames(bool numbered = false, bool incl_engine = false);
	GUI::ListData lpals();
	GUI::ListData subscreens(byte type, bool numbered = false);
	GUI::ListData disableditems(byte* disabledarray);
	//Scripts
	GUI::ListData itemdata_script();
	GUI::ListData itemsprite_script();
	GUI::ListData ffc_script();
	GUI::ListData dmap_script();
	GUI::ListData screen_script();
	GUI::ListData lweapon_script();
	GUI::ListData eweapon_script();
	GUI::ListData combodata_script();
	GUI::ListData generic_script();
	//const& returns
  GUI::ListData const& deftypes();
  GUI::ListData const& warpeffects();
  GUI::ListData const& screenstate();
  GUI::ListData const& subscr_widgets();
	GUI::ListData const& dmaptypes();
	GUI::ListData const& alignments();
	GUI::ListData const& buttons();
}

#endif
