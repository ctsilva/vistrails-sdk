/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Color.h"

static const QColor black = QColor(Qt::black);
QHash<QString, QColor> ColorByName::colors = QHash<QString, QColor>();

QColor ColorByName::get(const QString &color)
{
  static bool first = true;
  if(first)
  {
    first = false;
    ColorByName::initColorMap();
  }
  
  return ColorByName::colors.value(color, black);
}

void ColorByName::initColorMap()
{
  ColorByName::colors.insert("antique_white", QColor::fromRgbF(0.9804, 0.9216, 0.8431));
  ColorByName::colors.insert("azure", QColor::fromRgbF(0.9412, 1.0000, 1.0000));
  ColorByName::colors.insert("bisque", QColor::fromRgbF(1.0000, 0.8941, 0.7686));
  ColorByName::colors.insert("blanched_almond", QColor::fromRgbF(1.0000, 0.9216, 0.8039));
  ColorByName::colors.insert("cornsilk", QColor::fromRgbF(1.0000, 0.9725, 0.8627));
  ColorByName::colors.insert("eggshell", QColor::fromRgbF(0.9900, 0.9000, 0.7900));
  ColorByName::colors.insert("floral_white", QColor::fromRgbF(1.0000, 0.9804, 0.9412));
  ColorByName::colors.insert("gainsboro", QColor::fromRgbF(0.8627, 0.8627, 0.8627));
  ColorByName::colors.insert("ghost_white", QColor::fromRgbF(0.9725, 0.9725, 1.0000));
  ColorByName::colors.insert("honeydew", QColor::fromRgbF(0.9412, 1.0000, 0.9412));
  ColorByName::colors.insert("ivory", QColor::fromRgbF(1.0000, 1.0000, 0.9412));
  ColorByName::colors.insert("lavender", QColor::fromRgbF(0.9020, 0.9020, 0.9804));
  ColorByName::colors.insert("lavender_blush", QColor::fromRgbF(1.0000, 0.9412, 0.9608));
  ColorByName::colors.insert("lemon_chiffon", QColor::fromRgbF(1.0000, 0.9804, 0.8039));
  ColorByName::colors.insert("linen", QColor::fromRgbF(0.9804, 0.9412, 0.9020));
  ColorByName::colors.insert("mint_cream", QColor::fromRgbF(0.9608, 1.0000, 0.9804));
  ColorByName::colors.insert("misty_rose", QColor::fromRgbF(1.0000, 0.8941, 0.8824));
  ColorByName::colors.insert("moccasin", QColor::fromRgbF(1.0000, 0.8941, 0.7098));
  ColorByName::colors.insert("navajo_white", QColor::fromRgbF(1.0000, 0.8706, 0.6784)); 
  ColorByName::colors.insert("old_lace", QColor::fromRgbF(0.9922, 0.9608, 0.9020)); 
  ColorByName::colors.insert("papaya_whip", QColor::fromRgbF(1.0000, 0.9373, 0.8353)); 
  ColorByName::colors.insert("peach_puff", QColor::fromRgbF(1.0000, 0.8549, 0.7255)); 
  ColorByName::colors.insert("seashell", QColor::fromRgbF(1.0000, 0.9608, 0.9333)); 
  ColorByName::colors.insert("snow", QColor::fromRgbF(1.0000, 0.9804, 0.9804));
  ColorByName::colors.insert("thistle", QColor::fromRgbF(0.8471, 0.7490, 0.8471)); 
  ColorByName::colors.insert("titanium_white", QColor::fromRgbF(0.9900, 1.0000, 0.9400));
  ColorByName::colors.insert("wheat", QColor::fromRgbF(0.9608, 0.8706, 0.7020));
  ColorByName::colors.insert("white", QColor::fromRgbF(1.0000, 1.0000, 1.0000));
  ColorByName::colors.insert("white_smoke", QColor::fromRgbF(0.9608, 0.9608, 0.9608));
  ColorByName::colors.insert("zinc_white", QColor::fromRgbF(0.9900, 0.9700, 1.0000));

  // Greys
  ColorByName::colors.insert("cold_grey", QColor::fromRgbF(0.5000, 0.5400, 0.5300)); 
  ColorByName::colors.insert("light_dim_grey", QColor::fromRgbF(0.45, 0.45, 0.45));
  ColorByName::colors.insert("dim_grey", QColor::fromRgbF(0.4118, 0.4118, 0.4118));
  ColorByName::colors.insert("dark_dim_grey", QColor::fromRgbF(0.35, 0.35, 0.35));
  ColorByName::colors.insert("grey", QColor::fromRgbF(0.7529, 0.7529, 0.7529));
  ColorByName::colors.insert("very_light_grey", QColor::fromRgbF(0.95, 0.95, 0.95));
  ColorByName::colors.insert("light_grey", QColor::fromRgbF(0.8275, 0.8275, 0.8275));
  ColorByName::colors.insert("slate_grey", QColor::fromRgbF(0.4392, 0.5020, 0.5647));
  ColorByName::colors.insert("slate_grey_dark", QColor::fromRgbF(0.1843, 0.3098, 0.3098));
  ColorByName::colors.insert("slate_grey_light", QColor::fromRgbF(0.4667, 0.5333, 0.6000));
  ColorByName::colors.insert("warm_grey", QColor::fromRgbF(0.5000, 0.5000, 0.4100));

  // Blacks
  ColorByName::colors.insert("black", QColor::fromRgbF(0.0000, 0.0000, 0.0000)); 
  ColorByName::colors.insert("ivory_black", QColor::fromRgbF(0.1600, 0.1400, 0.1300));
  ColorByName::colors.insert("lamp_black", QColor::fromRgbF(0.1800, 0.2800, 0.2300));

  // Reds
  ColorByName::colors.insert("alizarin_crimson", QColor::fromRgbF(0.8900, 0.1500, 0.2100)); 
  ColorByName::colors.insert("brick", QColor::fromRgbF(0.6100, 0.4000, 0.1200));
  ColorByName::colors.insert("cadmium_red_deep", QColor::fromRgbF(0.8900, 0.0900, 0.0500));
  ColorByName::colors.insert("coral", QColor::fromRgbF(1.0000, 0.4980, 0.3137));
  ColorByName::colors.insert("coral_light", QColor::fromRgbF(0.9412, 0.5020, 0.5020));
  ColorByName::colors.insert("deep_pink", QColor::fromRgbF(1.0000, 0.0784, 0.5765)); 
  ColorByName::colors.insert("english_red", QColor::fromRgbF(0.8300, 0.2400, 0.1000)); 
  ColorByName::colors.insert("firebrick", QColor::fromRgbF(0.6980, 0.1333, 0.1333));
  ColorByName::colors.insert("geranium_lake", QColor::fromRgbF(0.8900, 0.0700, 0.1900));
  ColorByName::colors.insert("hot_pink", QColor::fromRgbF(1.0000, 0.4118, 0.7059)); 
  ColorByName::colors.insert("indian_red", QColor::fromRgbF(0.6900, 0.0900, 0.1200));
  ColorByName::colors.insert("light_salmon", QColor::fromRgbF(1.0000, 0.6275, 0.4784));
  ColorByName::colors.insert("madder_lake_deep", QColor::fromRgbF(0.8900, 0.1800, 0.1900));
  ColorByName::colors.insert("maroon", QColor::fromRgbF(0.6902, 0.1882, 0.3765));
  ColorByName::colors.insert("pink", QColor::fromRgbF(1.0000, 0.7529, 0.7961));
  ColorByName::colors.insert("pink_light", QColor::fromRgbF(1.0000, 0.7137, 0.7569));
  ColorByName::colors.insert("raspberry", QColor::fromRgbF(0.5300, 0.1500, 0.3400));
  ColorByName::colors.insert("red", QColor::fromRgbF(1.0000, 0.0000, 0.0000));
  ColorByName::colors.insert("rose_madder", QColor::fromRgbF(0.8900, 0.2100, 0.2200));
  ColorByName::colors.insert("salmon", QColor::fromRgbF(0.9804, 0.5020, 0.4471));
  ColorByName::colors.insert("tomato", QColor::fromRgbF(1.0000, 0.3882, 0.2784));
  ColorByName::colors.insert("venetian_red", QColor::fromRgbF(0.8300, 0.1000, 0.1200));

  // Browns
  ColorByName::colors.insert("beige", QColor::fromRgbF(0.6400, 0.5800, 0.5000));  
  ColorByName::colors.insert("brown", QColor::fromRgbF(0.5000, 0.1647, 0.1647));  
  ColorByName::colors.insert("brown_madder", QColor::fromRgbF(0.8600, 0.1600, 0.1600));  
  ColorByName::colors.insert("brown_ochre", QColor::fromRgbF(0.5300, 0.2600, 0.1200));  
  ColorByName::colors.insert("burlywood", QColor::fromRgbF(0.8706, 0.7216, 0.5294));  
  ColorByName::colors.insert("burnt_sienna", QColor::fromRgbF(0.5400, 0.2100, 0.0600));  
  ColorByName::colors.insert("burnt_umber", QColor::fromRgbF(0.5400, 0.2000, 0.1400));  
  ColorByName::colors.insert("chocolate", QColor::fromRgbF(0.8235, 0.4118, 0.1176));  
  ColorByName::colors.insert("deep_ochre", QColor::fromRgbF(0.4500, 0.2400, 0.1000));  
  ColorByName::colors.insert("flesh", QColor::fromRgbF(1.0000, 0.4900, 0.2500));  
  ColorByName::colors.insert("flesh_ochre", QColor::fromRgbF(1.0000, 0.3400, 0.1300));  
  ColorByName::colors.insert("gold_ochre", QColor::fromRgbF(0.7800, 0.4700, 0.1500));  
  ColorByName::colors.insert("greenish_umber", QColor::fromRgbF(1.0000, 0.2400, 0.0500));  
  ColorByName::colors.insert("khaki", QColor::fromRgbF(0.9412, 0.9020, 0.5490));  
  ColorByName::colors.insert("khaki_dark", QColor::fromRgbF(0.7412, 0.7176, 0.4196));  
  ColorByName::colors.insert("light_beige", QColor::fromRgbF(0.9608, 0.9608, 0.8627));  
  ColorByName::colors.insert("peru", QColor::fromRgbF(0.8039, 0.5216, 0.2471));  
  ColorByName::colors.insert("rosy_brown", QColor::fromRgbF(0.7373, 0.5608, 0.5608));  
  ColorByName::colors.insert("raw_sienna", QColor::fromRgbF(0.7800, 0.3800, 0.0800));  
  ColorByName::colors.insert("raw_umber", QColor::fromRgbF(0.4500, 0.2900, 0.0700));  
  ColorByName::colors.insert("sepia", QColor::fromRgbF(0.3700, 0.1500, 0.0700));  
  ColorByName::colors.insert("sienna", QColor::fromRgbF(0.6275, 0.3216, 0.1765));  
  ColorByName::colors.insert("saddle_brown", QColor::fromRgbF(0.5451, 0.2706, 0.0745));  
  ColorByName::colors.insert("sandy_brown", QColor::fromRgbF(0.9569, 0.6431, 0.3765));  
  ColorByName::colors.insert("tan", QColor::fromRgbF(0.8235, 0.7059, 0.5490));  
  ColorByName::colors.insert("van_dyke_brown", QColor::fromRgbF(0.3700, 0.1500, 0.0200));  

  // Oranges
  ColorByName::colors.insert("cadmium_orange", QColor::fromRgbF(1.0000, 0.3800, 0.0100));  
  ColorByName::colors.insert("cadmium_red_light", QColor::fromRgbF(1.0000, 0.0100, 0.0500));  
  ColorByName::colors.insert("carrot", QColor::fromRgbF(0.9300, 0.5700, 0.1300));  
  ColorByName::colors.insert("dark_orange", QColor::fromRgbF(1.0000, 0.5490, 0.0000));  
  ColorByName::colors.insert("mars_orange", QColor::fromRgbF(0.5900, 0.2700, 0.0800));  
  ColorByName::colors.insert("mars_yellow", QColor::fromRgbF(0.8900, 0.4400, 0.1000));  
  ColorByName::colors.insert("orange", QColor::fromRgbF(1.0000, 0.5000, 0.0000));  
  ColorByName::colors.insert("orange_red", QColor::fromRgbF(1.0000, 0.2706, 0.0000));  
  ColorByName::colors.insert("yellow_ochre", QColor::fromRgbF(0.8900, 0.5100, 0.0900));  

  // Yellows
  ColorByName::colors.insert("aureoline_yellow", QColor::fromRgbF(1.0000, 0.6600, 0.1400));  
  ColorByName::colors.insert("banana", QColor::fromRgbF(0.8900, 0.8100, 0.3400));  
  ColorByName::colors.insert("cadmium_lemon", QColor::fromRgbF(1.0000, 0.8900, 0.0100));  
  ColorByName::colors.insert("cadmium_yellow", QColor::fromRgbF(1.0000, 0.6000, 0.0700));  
  ColorByName::colors.insert("cadmium_yellow_light", QColor::fromRgbF(1.0000, 0.6900, 0.0600));  
  ColorByName::colors.insert("gold", QColor::fromRgbF(1.0000, 0.8431, 0.0000));  
  ColorByName::colors.insert("goldenrod", QColor::fromRgbF(0.8549, 0.6471, 0.1255));  
  ColorByName::colors.insert("goldenrod_medium", QColor::fromRgbF(0.8549, 0.6971, 0.2255));
  ColorByName::colors.insert("goldenrod_dark", QColor::fromRgbF(0.7216, 0.5255, 0.0431));  
  ColorByName::colors.insert("goldenrod_light", QColor::fromRgbF(0.9804, 0.9804, 0.8235));  
  ColorByName::colors.insert("goldenrod_pale", QColor::fromRgbF(0.9333, 0.9098, 0.6667));  
  ColorByName::colors.insert("light_goldenrod", QColor::fromRgbF(0.9333, 0.8667, 0.5098));  
  ColorByName::colors.insert("melon", QColor::fromRgbF(0.8900, 0.6600, 0.4100));  
  ColorByName::colors.insert("naples_yellow_deep", QColor::fromRgbF(1.0000, 0.6600, 0.0700));  
  ColorByName::colors.insert("yellow", QColor::fromRgbF(1.0000, 1.0000, 0.0000));  
  ColorByName::colors.insert("yellow_light", QColor::fromRgbF(1.0000, 1.0000, 0.8784));  

  // Greens
  ColorByName::colors.insert("chartreuse", QColor::fromRgbF(0.4980, 1.0000, 0.0000));  
  ColorByName::colors.insert("chrome_oxide_green", QColor::fromRgbF(0.4000, 0.5000, 0.0800));  
  ColorByName::colors.insert("cinnabar_green", QColor::fromRgbF(0.3800, 0.7000, 0.1600));  
  ColorByName::colors.insert("cobalt_green", QColor::fromRgbF(0.2400, 0.5700, 0.2500));  
  ColorByName::colors.insert("emerald_green", QColor::fromRgbF(0.0000, 0.7900, 0.3400));  
  ColorByName::colors.insert("forest_green", QColor::fromRgbF(0.1333, 0.5451, 0.1333));  
  ColorByName::colors.insert("green", QColor::fromRgbF(0.0000, 1.0000, 0.0000));  
  ColorByName::colors.insert("green_dark", QColor::fromRgbF(0.0000, 0.3922, 0.0000));  
  ColorByName::colors.insert("green_pale", QColor::fromRgbF(0.5961, 0.9843, 0.5961));  
  ColorByName::colors.insert("green_yellow", QColor::fromRgbF(0.6784, 1.0000, 0.1843));  
  ColorByName::colors.insert("lawn_green", QColor::fromRgbF(0.4863, 0.9882, 0.0000));  
  ColorByName::colors.insert("lime_green", QColor::fromRgbF(0.1961, 0.8039, 0.1961));  
  ColorByName::colors.insert("mint", QColor::fromRgbF(0.7400, 0.9900, 0.7900));  
  ColorByName::colors.insert("olive", QColor::fromRgbF(0.2300, 0.3700, 0.1700));  
  ColorByName::colors.insert("olive_drab", QColor::fromRgbF(0.4196, 0.5569, 0.1373));  
  ColorByName::colors.insert("olive_green_dark", QColor::fromRgbF(0.3333, 0.4196, 0.1843));  
  ColorByName::colors.insert("permanent_green", QColor::fromRgbF(0.0400, 0.7900, 0.1700));  
  ColorByName::colors.insert("sap_green", QColor::fromRgbF(0.1900, 0.5000, 0.0800));  
  ColorByName::colors.insert("sea_green", QColor::fromRgbF(0.1804, 0.5451, 0.3412));  
  ColorByName::colors.insert("sea_green_dark", QColor::fromRgbF(0.5608, 0.7373, 0.5608));  
  ColorByName::colors.insert("sea_green_medium", QColor::fromRgbF(0.2353, 0.7020, 0.4431));  
  ColorByName::colors.insert("sea_green_light", QColor::fromRgbF(0.1255, 0.6980, 0.6667));  
  ColorByName::colors.insert("spring_green", QColor::fromRgbF(0.0000, 1.0000, 0.4980));  
  ColorByName::colors.insert("spring_green_medium", QColor::fromRgbF(0.0000, 0.9804, 0.6039));  
  ColorByName::colors.insert("terre_verte", QColor::fromRgbF(0.2200, 0.3700, 0.0600));  
  ColorByName::colors.insert("viridian_light", QColor::fromRgbF(0.4300, 1.0000, 0.4400));  
  ColorByName::colors.insert("yellow_green", QColor::fromRgbF(0.6039, 0.8039, 0.1961));  

  // Cyans
  ColorByName::colors.insert("aquamarine", QColor::fromRgbF(0.4980, 1.0000, 0.8314));  
  ColorByName::colors.insert("aquamarine_medium", QColor::fromRgbF(0.4000, 0.8039, 0.6667));  
  ColorByName::colors.insert("cyan", QColor::fromRgbF(0.0000, 1.0000, 1.0000));  
  ColorByName::colors.insert("cyan_white", QColor::fromRgbF(0.8784, 1.0000, 1.0000));  
  ColorByName::colors.insert("turquoise", QColor::fromRgbF(0.2510, 0.8784, 0.8157));  
  ColorByName::colors.insert("turquoise_dark", QColor::fromRgbF(0.0000, 0.8078, 0.8196));  
  ColorByName::colors.insert("turquoise_medium", QColor::fromRgbF(0.2824, 0.8196, 0.8000));  
  ColorByName::colors.insert("turquoise_pale", QColor::fromRgbF(0.6863, 0.9333, 0.9333));  

  // Blues
  ColorByName::colors.insert("alice_blue", QColor::fromRgbF(0.9412, 0.9725, 1.0000));  
  ColorByName::colors.insert("blue", QColor::fromRgbF(0.0000, 0.0000, 1.0000));  
  ColorByName::colors.insert("blue_light", QColor::fromRgbF(0.6784, 0.8471, 0.9020));  
  ColorByName::colors.insert("blue_medium", QColor::fromRgbF(0.0000, 0.0000, 0.8039));  
  ColorByName::colors.insert("cadet", QColor::fromRgbF(0.3725, 0.6196, 0.6275));  
  ColorByName::colors.insert("cobalt", QColor::fromRgbF(0.2400, 0.3500, 0.6700));  
  ColorByName::colors.insert("cornflower", QColor::fromRgbF(0.3922, 0.5843, 0.9294));  
  ColorByName::colors.insert("desatcornflower", QColor::fromRgbF(0.5138, 0.6098, 0.7835));  
  ColorByName::colors.insert("cerulean", QColor::fromRgbF(0.0200, 0.7200, 0.8000));  
  ColorByName::colors.insert("dodger_blue", QColor::fromRgbF(0.1176, 0.5647, 1.0000));  
  ColorByName::colors.insert("indigo", QColor::fromRgbF(0.0300, 0.1800, 0.3300));  
  ColorByName::colors.insert("manganese_blue", QColor::fromRgbF(0.0100, 0.6600, 0.6200));  
  ColorByName::colors.insert("midnight_blue", QColor::fromRgbF(0.0980, 0.0980, 0.4392));  
  ColorByName::colors.insert("navy", QColor::fromRgbF(0.0000, 0.0000, 0.5020));  
  ColorByName::colors.insert("peacock", QColor::fromRgbF(0.2000, 0.6300, 0.7900));  
  ColorByName::colors.insert("powder_blue", QColor::fromRgbF(0.6902, 0.8784, 0.9020));  
  ColorByName::colors.insert("royal_blue", QColor::fromRgbF(0.2549, 0.4118, 0.8824));  
  ColorByName::colors.insert("slate_blue", QColor::fromRgbF(0.4157, 0.3529, 0.8039));  
  ColorByName::colors.insert("slate_blue_dark", QColor::fromRgbF(0.2824, 0.2392, 0.5451));  
  ColorByName::colors.insert("slate_blue_light", QColor::fromRgbF(0.5176, 0.4392, 1.0000));  
  ColorByName::colors.insert("slate_blue_medium", QColor::fromRgbF(0.4824, 0.4078, 0.9333));  
  ColorByName::colors.insert("sky_blue", QColor::fromRgbF(0.5294, 0.8078, 0.9216));  
  ColorByName::colors.insert("sky_blue_deep", QColor::fromRgbF(0.0000, 0.7490, 1.0000));  
  ColorByName::colors.insert("sky_blue_light", QColor::fromRgbF(0.5294, 0.8078, 0.9804));  
  ColorByName::colors.insert("steel_blue", QColor::fromRgbF(0.2745, 0.5098, 0.7059));  
  ColorByName::colors.insert("steel_blue_light", QColor::fromRgbF(0.6902, 0.7686, 0.8706));  
  ColorByName::colors.insert("turquoise_blue", QColor::fromRgbF(0.0000, 0.7800, 0.5500));  
  ColorByName::colors.insert("ultramarine", QColor::fromRgbF(0.0700, 0.0400, 0.5600));  

  // Magentas
  ColorByName::colors.insert("blue_violet", QColor::fromRgbF(0.5412, 0.1686, 0.8863));  
  ColorByName::colors.insert("cobalt_violet_deep", QColor::fromRgbF(0.5700, 0.1300, 0.6200));  
  ColorByName::colors.insert("magenta", QColor::fromRgbF(1.0000, 0.0000, 1.0000));  
  ColorByName::colors.insert("orchid", QColor::fromRgbF(0.8549, 0.4392, 0.8392));  
  ColorByName::colors.insert("orchid_dark", QColor::fromRgbF(0.6000, 0.1961, 0.8000));  
  ColorByName::colors.insert("orchid_medium", QColor::fromRgbF(0.7294, 0.3333, 0.8275));  
  ColorByName::colors.insert("permanent_red_violet", QColor::fromRgbF(0.8600, 0.1500, 0.2700));  
  ColorByName::colors.insert("plum", QColor::fromRgbF(0.8667, 0.6275, 0.8667));  
  ColorByName::colors.insert("purple", QColor::fromRgbF(0.6275, 0.1255, 0.9412));  
  ColorByName::colors.insert("purple_medium", QColor::fromRgbF(0.5765, 0.4392, 0.8588));  
  ColorByName::colors.insert("ultramarine_violet", QColor::fromRgbF(0.3600, 0.1400, 0.4300));  
  ColorByName::colors.insert("violet", QColor::fromRgbF(0.5600, 0.3700, 0.6000));  
  ColorByName::colors.insert("violet_dark", QColor::fromRgbF(0.5804, 0.0000, 0.8275));  
  ColorByName::colors.insert("violet_red", QColor::fromRgbF(0.8157, 0.1255, 0.5647));  
  ColorByName::colors.insert("violet_red_medium", QColor::fromRgbF(0.7804, 0.0824, 0.5216));  
  ColorByName::colors.insert("violet_red_pale", QColor::fromRgbF(0.8588, 0.4392, 0.5765));
}
