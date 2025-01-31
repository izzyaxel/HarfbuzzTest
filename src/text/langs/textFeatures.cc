#include "textFeatures.hh"

#include <limits>
#include <harfbuzz/hb.h>

constexpr hb_tag_t kerningTag = HB_TAG('k','e','r','n');
constexpr hb_tag_t ligaturesTag = HB_TAG('l','i','g','a');
constexpr hb_tag_t contextualLigaturesTag = HB_TAG('c','l','i','g');

hb_feature_t ligaturesOff = {ligaturesTag, 0, 0, std::numeric_limits<uint8_t>::max()};
hb_feature_t ligaturesOn = {ligaturesTag, 1, 0, std::numeric_limits<uint8_t>::max()};
hb_feature_t contextualLigaturesOff = {contextualLigaturesTag, 0, 0, std::numeric_limits<uint8_t>::max()};
hb_feature_t contextualLigaturesOn = {contextualLigaturesTag, 1, 0, std::numeric_limits<uint8_t>::max()};
hb_feature_t kerningOff = {kerningTag, 0, 0, std::numeric_limits<uint8_t>::max()};
hb_feature_t kerningOn = {kerningTag, 1, 0, std::numeric_limits<uint8_t>::max()};
