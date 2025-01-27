#pragma once

#include "languages.hh"

#include <harfbuzz/hb.h>

struct Language
{
  std::string lang;
  hb_script_t script{};
  hb_direction_t direction{};
};

//TODO
inline Language EnglishLang{english, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
inline Language FrenchLang{french, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
inline Language RussianLang{russian, HB_SCRIPT_CYRILLIC, HB_DIRECTION_LTR};
inline Language GermanLang{german, HB_SCRIPT_GOTHIC, HB_DIRECTION_LTR};
inline Language ItalianLang{italian, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
inline Language SpanishLang{spanish, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
//inline Language JapaneseLang;
//inline Language ChineseLang;

inline Language UkrainianLang{ukranian, HB_SCRIPT_CYRILLIC, HB_DIRECTION_LTR};
inline Language KoreanLang{korean, HB_SCRIPT_HANGUL, HB_DIRECTION_LTR};
inline Language DutchLang{dutch, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
inline Language PortugueseLang{portugese, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
inline Language ArabicLang{arabic, HB_SCRIPT_ARABIC, HB_DIRECTION_RTL};
inline Language ArmenianLang{armenian, HB_SCRIPT_ARMENIAN, HB_DIRECTION_LTR};
inline Language GreekLang{greek, HB_SCRIPT_GREEK, HB_DIRECTION_LTR};
inline Language HebrewLang{hebrew, HB_SCRIPT_HEBREW, HB_DIRECTION_RTL};
inline Language LatinLang{latin, HB_SCRIPT_LATIN, HB_DIRECTION_LTR};
