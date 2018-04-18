#ifndef __Qvtk_IMAGE_LABEL_H__
#define __Qvtk_IMAGE_LABEL_H__

// me
#include "QvtkImage.h"

// vtk 
class vtkLookupTable;
class vtkImageMapToColors;
class vtkNamedColors;
class vtkPassThroughFilter;
#include <vtkSmartPointer.h>
/**
 * http://htmlpreview.github.io/?https://github.com/lorensen/VTKExamples/blob/master/src/Python/Visualization/VTKNamedColorPatches.html
 * id: 0   Name: NULL			   RGBA: (0, 0, 0, 0)
 * id: 1   Name: alice_blue        RGBA: (0.941176, 0.972549, 1, 1)
 * id: 2   Name: aliceblue RGBA: (0.941176, 0.972549, 1, 1)
 * id: 3   Name: alizarin_crimson  RGBA: (0.890196, 0.14902, 0.211765, 1)
 * id: 4   Name: antique_white     RGBA: (0.980392, 0.921569, 0.843137, 1)
 * id: 5   Name: antiquewhite      RGBA: (0.980392, 0.921569, 0.843137, 1)
 * id: 6   Name: aqua      RGBA: (0, 1, 1, 1)
 * id: 7   Name: aquamarine        RGBA: (0.498039, 1, 0.831373, 1)
 * id: 8   Name: aquamarine_medium RGBA: (0.4, 0.803922, 0.666667, 1)
 * id: 9   Name: aureoline_yellow  RGBA: (1, 0.658824, 0.141176, 1)
 * id: 10  Name: azure     RGBA: (0.941176, 1, 1, 1)
 * id: 11  Name: banana    RGBA: (0.890196, 0.811765, 0.341176, 1)
 * id: 12  Name: beige     RGBA: (0.960784, 0.960784, 0.862745, 1)
 * id: 13  Name: bisque    RGBA: (1, 0.894118, 0.768627, 1)
 * id: 14  Name: black     RGBA: (0, 0, 0, 1)
 * id: 15  Name: blanched_almond   RGBA: (1, 0.921569, 0.803922, 1)
 * id: 16  Name: blanchedalmond    RGBA: (1, 0.921569, 0.803922, 1)
 * id: 17  Name: blue      RGBA: (0, 0, 1, 1)
 * id: 18  Name: blue_light        RGBA: (0.678431, 0.847059, 0.901961, 1)
 * id: 19  Name: blue_medium       RGBA: (0, 0, 0.803922, 1)
 * id: 20  Name: blue_violet       RGBA: (0.541176, 0.168627, 0.886275, 1)
 * id: 21  Name: blueviolet        RGBA: (0.541176, 0.168627, 0.886275, 1)
 * id: 22  Name: brick     RGBA: (0.611765, 0.4, 0.121569, 1)
 * id: 23  Name: brown     RGBA: (0.647059, 0.164706, 0.164706, 1)
 * id: 24  Name: brown_madder      RGBA: (0.858824, 0.160784, 0.160784, 1)
 * id: 25  Name: brown_ochre       RGBA: (0.529412, 0.258824, 0.121569, 1)
 * id: 26  Name: burlywood RGBA: (0.870588, 0.721569, 0.529412, 1)
 * id: 27  Name: burnt_sienna      RGBA: (0.541176, 0.211765, 0.0588235, 1)
 * id: 28  Name: burnt_umber       RGBA: (0.541176, 0.2, 0.141176, 1)
 * id: 29  Name: cadet     RGBA: (0.372549, 0.619608, 0.627451, 1)
 * id: 30  Name: cadetblue RGBA: (0.372549, 0.619608, 0.627451, 1)
 * id: 31  Name: cadmiulemon     RGBA: (1, 0.890196, 0.0117647, 1)
 * id: 32  Name: cadmiuorange    RGBA: (1, 0.380392, 0.0117647, 1)
 * id: 33  Name: cadmiured_deep  RGBA: (0.890196, 0.0901961, 0.0509804, 1)
 * id: 34  Name: cadmiured_light RGBA: (1, 0.0117647, 0.0509804, 1)
 * id: 35  Name: cadmiuyellow    RGBA: (1, 0.6, 0.0705882, 1)
 * id: 36  Name: cadmiuyellow_light      RGBA: (1, 0.690196, 0.0588235, 1)
 * id: 37  Name: carrot    RGBA: (0.929412, 0.568627, 0.129412, 1)
 * id: 38  Name: cerulean  RGBA: (0.0196078, 0.721569, 0.8, 1)
 * id: 39  Name: chartreuse        RGBA: (0.498039, 1, 0, 1)
 * id: 40  Name: chocolate RGBA: (0.823529, 0.411765, 0.117647, 1)
 * id: 41  Name: chrome_oxide_green        RGBA: (0.4, 0.501961, 0.0784314, 1)
 * id: 42  Name: cinnabar_green    RGBA: (0.380392, 0.701961, 0.160784, 1)
 * id: 43  Name: cobalt    RGBA: (0.239216, 0.34902, 0.670588, 1)
 * id: 44  Name: cobalt_green      RGBA: (0.239216, 0.568627, 0.25098, 1)
 * id: 45  Name: cobalt_violet_deep        RGBA: (0.568627, 0.129412, 0.619608, 1)
 * id: 46  Name: cold_grey RGBA: (0.501961, 0.541176, 0.529412, 1)
 * id: 47  Name: coral     RGBA: (1, 0.498039, 0.313725, 1)
 * id: 48  Name: coral_light       RGBA: (0.941176, 0.501961, 0.501961, 1)
 * id: 49  Name: cornflower        RGBA: (0.392157, 0.584314, 0.929412, 1)
 * id: 50  Name: cornflowerblue    RGBA: (0.392157, 0.584314, 0.929412, 1)
 * id: 51  Name: cornsilk  RGBA: (1, 0.972549, 0.862745, 1)
 * id: 52  Name: crimson   RGBA: (0.862745, 0.0784314, 0.235294, 1)
 * id: 53  Name: cyan      RGBA: (0, 1, 1, 1)
 * id: 54  Name: cyan_white        RGBA: (0.878431, 1, 1, 1)
 * id: 55  Name: dark_orange       RGBA: (1, 0.54902, 0, 1)
 * id: 56  Name: darkblue  RGBA: (0, 0, 0.545098, 1)
 * id: 57  Name: darkcyan  RGBA: (0, 0.545098, 0.545098, 1)
 * id: 58  Name: darkgoldenrod     RGBA: (0.721569, 0.52549, 0.0431373, 1)
 * id: 59  Name: darkgray  RGBA: (0.662745, 0.662745, 0.662745, 1)
 * id: 60  Name: darkgreen RGBA: (0, 0.392157, 0, 1)
 * id: 61  Name: darkkhaki RGBA: (0.741176, 0.717647, 0.419608, 1)
 * id: 62  Name: darkmagenta       RGBA: (0.545098, 0, 0.545098, 1)
 * id: 63  Name: darkolivegreen    RGBA: (0.333333, 0.419608, 0.184314, 1)
 * id: 64  Name: darkorange        RGBA: (1, 0.54902, 0, 1)
 * id: 65  Name: darkorchid        RGBA: (0.6, 0.196078, 0.8, 1)
 * id: 66  Name: darkred   RGBA: (0.545098, 0, 0, 1)
 * id: 67  Name: darksalmon        RGBA: (0.913725, 0.588235, 0.478431, 1)
 * id: 68  Name: darkseagreen      RGBA: (0.560784, 0.737255, 0.560784, 1)
 * id: 69  Name: darkslateblue     RGBA: (0.282353, 0.239216, 0.545098, 1)
 * id: 70  Name: darkslategray     RGBA: (0.184314, 0.309804, 0.309804, 1)
 * id: 71  Name: darkturquoise     RGBA: (0, 0.807843, 0.819608, 1)
 * id: 72  Name: darkviolet        RGBA: (0.580392, 0, 0.827451, 1)
 * id: 73  Name: deep_ochre        RGBA: (0.45098, 0.239216, 0.101961, 1)
 * id: 74  Name: deep_pink RGBA: (1, 0.0784314, 0.576471, 1)
 * id: 75  Name: deeppink  RGBA: (1, 0.0784314, 0.576471, 1)
 * id: 76  Name: deepskyblue       RGBA: (0, 0.74902, 1, 1)
 * id: 77  Name: digrey  RGBA: (0.411765, 0.411765, 0.411765, 1)
 * id: 78  Name: dimgray   RGBA: (0.411765, 0.411765, 0.411765, 1)
 * id: 79  Name: dodger_blue       RGBA: (0.117647, 0.564706, 1, 1)
 * id: 80  Name: dodgerblue        RGBA: (0.117647, 0.564706, 1, 1)
 * id: 81  Name: eggshell  RGBA: (0.988235, 0.901961, 0.788235, 1)
 * id: 82  Name: emerald_green     RGBA: (0, 0.788235, 0.341176, 1)
 * id: 83  Name: english_red       RGBA: (0.831373, 0.239216, 0.101961, 1)
 * id: 84  Name: firebrick RGBA: (0.698039, 0.133333, 0.133333, 1)
 * id: 85  Name: flesh     RGBA: (1, 0.490196, 0.25098, 1)
 * id: 86  Name: flesh_ochre       RGBA: (1, 0.341176, 0.129412, 1)
 * id: 87  Name: floral_white      RGBA: (1, 0.980392, 0.941176, 1)
 * id: 88  Name: floralwhite       RGBA: (1, 0.980392, 0.941176, 1)
 * id: 89  Name: forest_green      RGBA: (0.133333, 0.545098, 0.133333, 1)
 * id: 90  Name: forestgreen       RGBA: (0.133333, 0.545098, 0.133333, 1)
 * id: 91  Name: fuchsia   RGBA: (1, 0, 1, 1)
 * id: 92  Name: gainsboro RGBA: (0.862745, 0.862745, 0.862745, 1)
 * id: 93  Name: geraniulake     RGBA: (0.890196, 0.0705882, 0.188235, 1)
 * id: 94  Name: ghost_white       RGBA: (0.972549, 0.972549, 1, 1)
 * id: 95  Name: ghostwhite        RGBA: (0.972549, 0.972549, 1, 1)
 * id: 96  Name: gold      RGBA: (1, 0.843137, 0, 1)
 * id: 97  Name: gold_ochre        RGBA: (0.780392, 0.470588, 0.14902, 1)
 * id: 98  Name: goldenrod RGBA: (0.854902, 0.647059, 0.12549, 1)
 * id: 99  Name: goldenrod_dark    RGBA: (0.721569, 0.52549, 0.0431373, 1)
 * id: 100 Name: goldenrod_light   RGBA: (0.980392, 0.980392, 0.823529, 1)
 * id: 101 Name: goldenrod_pale    RGBA: (0.933333, 0.909804, 0.666667, 1)
 * id: 102 Name: gray      RGBA: (0.501961, 0.501961, 0.501961, 1)
 * id: 103 Name: green     RGBA: (0, 0.501961, 0, 1)
 * id: 104 Name: green_dark        RGBA: (0, 0.392157, 0, 1)
 * id: 105 Name: green_pale        RGBA: (0.596078, 0.984314, 0.596078, 1)
 * id: 106 Name: green_yellow      RGBA: (0.678431, 1, 0.184314, 1)
 * id: 107 Name: greenish_umber    RGBA: (1, 0.239216, 0.0509804, 1)
 * id: 108 Name: greenyellow       RGBA: (0.678431, 1, 0.184314, 1)
 * id: 109 Name: grey      RGBA: (0.752941, 0.752941, 0.752941, 1)
 * id: 110 Name: honeydew  RGBA: (0.941176, 1, 0.941176, 1)
 * id: 111 Name: hot_pink  RGBA: (1, 0.411765, 0.705882, 1)
 * id: 112 Name: hotpink   RGBA: (1, 0.411765, 0.705882, 1)
 * id: 113 Name: indian_red        RGBA: (0.690196, 0.0901961, 0.121569, 1)
 * id: 114 Name: indianred RGBA: (0.803922, 0.360784, 0.360784, 1)
 * id: 115 Name: indigo    RGBA: (0.294118, 0, 0.509804, 1)
 * id: 116 Name: ivory     RGBA: (1, 1, 0.941176, 1)
 * id: 117 Name: ivory_black       RGBA: (0.160784, 0.141176, 0.129412, 1)
 * id: 118 Name: khaki     RGBA: (0.941176, 0.901961, 0.54902, 1)
 * id: 119 Name: khaki_dark        RGBA: (0.741176, 0.717647, 0.419608, 1)
 * id: 120 Name: lamp_black        RGBA: (0.180392, 0.278431, 0.231373, 1)
 * id: 121 Name: lavender  RGBA: (0.901961, 0.901961, 0.980392, 1)
 * id: 122 Name: lavender_blush    RGBA: (1, 0.941176, 0.960784, 1)
 * id: 123 Name: lavenderblush     RGBA: (1, 0.941176, 0.960784, 1)
 * id: 124 Name: lawn_green        RGBA: (0.486275, 0.988235, 0, 1)
 * id: 125 Name: lawngreen RGBA: (0.486275, 0.988235, 0, 1)
 * id: 126 Name: lemon_chiffon     RGBA: (1, 0.980392, 0.803922, 1)
 * id: 127 Name: lemonchiffon      RGBA: (1, 0.980392, 0.803922, 1)
 * id: 128 Name: light_beige       RGBA: (0.960784, 0.960784, 0.862745, 1)
 * id: 129 Name: light_goldenrod   RGBA: (0.933333, 0.866667, 0.509804, 1)
 * id: 130 Name: light_grey        RGBA: (0.827451, 0.827451, 0.827451, 1)
 * id: 131 Name: light_salmon      RGBA: (1, 0.627451, 0.478431, 1)
 * id: 132 Name: lightblue RGBA: (0.678431, 0.847059, 0.901961, 1)
 * id: 133 Name: lightcoral        RGBA: (0.941176, 0.501961, 0.501961, 1)
 * id: 134 Name: lightcyan RGBA: (0.878431, 1, 1, 1)
 * id: 135 Name: lightgoldenrodyellow      RGBA: (0.980392, 0.980392, 0.823529, 1)
 * id: 136 Name: lightgreen        RGBA: (0.564706, 0.933333, 0.564706, 1)
 * id: 137 Name: lightgrey RGBA: (0.827451, 0.827451, 0.827451, 1)
 * id: 138 Name: lightpink RGBA: (1, 0.713725, 0.756863, 1)
 * id: 139 Name: lightsalmon       RGBA: (1, 0.627451, 0.478431, 1)
 * id: 140 Name: lightseagreen     RGBA: (0.12549, 0.698039, 0.666667, 1)
 * id: 141 Name: lightskyblue      RGBA: (0.529412, 0.807843, 0.980392, 1)
 * id: 142 Name: lightslategray    RGBA: (0.466667, 0.533333, 0.6, 1)
 * id: 143 Name: lightsteelblue    RGBA: (0.690196, 0.768627, 0.870588, 1)
 * id: 144 Name: lightyellow       RGBA: (1, 1, 0.878431, 1)
 * id: 145 Name: lime      RGBA: (0, 1, 0, 1)
 * id: 146 Name: lime_green        RGBA: (0.196078, 0.803922, 0.196078, 1)
 * id: 147 Name: limegreen RGBA: (0.196078, 0.803922, 0.196078, 1)
 * id: 148 Name: linen     RGBA: (0.980392, 0.941176, 0.901961, 1)
 * id: 149 Name: madder_lake_deep  RGBA: (0.890196, 0.180392, 0.188235, 1)
 * id: 150 Name: magenta   RGBA: (1, 0, 1, 1)
 * id: 151 Name: manganese_blue    RGBA: (0.0117647, 0.658824, 0.619608, 1)
 * id: 152 Name: maroon    RGBA: (0.501961, 0, 0, 1)
 * id: 153 Name: mars_orange       RGBA: (0.588235, 0.270588, 0.0784314, 1)
 * id: 154 Name: mars_yellow       RGBA: (0.890196, 0.439216, 0.101961, 1)
 * id: 155 Name: mediumaquamarine  RGBA: (0.4, 0.803922, 0.666667, 1)
 * id: 156 Name: mediumblue        RGBA: (0, 0, 0.803922, 1)
 * id: 157 Name: mediumorchid      RGBA: (0.729412, 0.333333, 0.827451, 1)
 * id: 158 Name: mediumpurple      RGBA: (0.576471, 0.439216, 0.858824, 1)
 * id: 159 Name: mediumseagreen    RGBA: (0.235294, 0.701961, 0.443137, 1)
 * id: 160 Name: mediumslateblue   RGBA: (0.482353, 0.407843, 0.933333, 1)
 * id: 161 Name: mediumspringgreen RGBA: (0, 0.980392, 0.603922, 1)
 * id: 162 Name: mediumturquoise   RGBA: (0.282353, 0.819608, 0.8, 1)
 * id: 163 Name: mediumvioletred   RGBA: (0.780392, 0.0823529, 0.521569, 1)
 * id: 164 Name: melon     RGBA: (0.890196, 0.658824, 0.411765, 1)
 * id: 165 Name: midnight_blue     RGBA: (0.0980392, 0.0980392, 0.439216, 1)
 * id: 166 Name: midnightblue      RGBA: (0.0980392, 0.0980392, 0.439216, 1)
 * id: 167 Name: mint      RGBA: (0.741176, 0.988235, 0.788235, 1)
 * id: 168 Name: mint_cream        RGBA: (0.960784, 1, 0.980392, 1)
 * id: 169 Name: mintcream RGBA: (0.960784, 1, 0.980392, 1)
 * id: 170 Name: misty_rose        RGBA: (1, 0.894118, 0.882353, 1)
 * id: 171 Name: mistyrose RGBA: (1, 0.894118, 0.882353, 1)
 * id: 172 Name: moccasin  RGBA: (1, 0.894118, 0.709804, 1)
 * id: 173 Name: naples_yellow_deep        RGBA: (1, 0.658824, 0.0705882, 1)
 * id: 174 Name: navajo_white      RGBA: (1, 0.870588, 0.678431, 1)
 * id: 175 Name: navajowhite       RGBA: (1, 0.870588, 0.678431, 1)
 * id: 176 Name: navy      RGBA: (0, 0, 0.501961, 1)
 * id: 177 Name: old_lace  RGBA: (0.992157, 0.960784, 0.901961, 1)
 * id: 178 Name: oldlace   RGBA: (0.992157, 0.960784, 0.901961, 1)
 * id: 179 Name: olive     RGBA: (0.501961, 0.501961, 0, 1)
 * id: 180 Name: olive_drab        RGBA: (0.419608, 0.556863, 0.137255, 1)
 * id: 181 Name: olive_green_dark  RGBA: (0.333333, 0.419608, 0.184314, 1)
 * id: 182 Name: olivedrab RGBA: (0.419608, 0.556863, 0.137255, 1)
 * id: 183 Name: orange    RGBA: (1, 0.647059, 0, 1)
 * id: 184 Name: orange_red        RGBA: (1, 0.270588, 0, 1)
 * id: 185 Name: orangered RGBA: (1, 0.270588, 0, 1)
 * id: 186 Name: orchid    RGBA: (0.854902, 0.439216, 0.839216, 1)
 * id: 187 Name: orchid_dark       RGBA: (0.6, 0.196078, 0.8, 1)
 * id: 188 Name: orchid_medium     RGBA: (0.729412, 0.333333, 0.827451, 1)
 * id: 189 Name: palegoldenrod     RGBA: (0.933333, 0.909804, 0.666667, 1)
 * id: 190 Name: palegreen RGBA: (0.596078, 0.984314, 0.596078, 1)
 * id: 191 Name: paleturquoise     RGBA: (0.686275, 0.933333, 0.933333, 1)
 * id: 192 Name: palevioletred     RGBA: (0.858824, 0.439216, 0.576471, 1)
 * id: 193 Name: papaya_whip       RGBA: (1, 0.937255, 0.835294, 1)
 * id: 194 Name: papayawhip        RGBA: (1, 0.937255, 0.835294, 1)
 * id: 195 Name: peach_puff        RGBA: (1, 0.854902, 0.72549, 1)
 * id: 196 Name: peachpuff RGBA: (1, 0.854902, 0.72549, 1)
 * id: 197 Name: peacock   RGBA: (0.2, 0.631373, 0.788235, 1)
 * id: 198 Name: permanent_green   RGBA: (0.0392157, 0.788235, 0.168627, 1)
 * id: 199 Name: permanent_red_violet      RGBA: (0.858824, 0.14902, 0.270588, 1)
 * id: 200 Name: peru      RGBA: (0.803922, 0.521569, 0.247059, 1)
 * id: 201 Name: pink      RGBA: (1, 0.752941, 0.796078, 1)
 * id: 202 Name: pink_light        RGBA: (1, 0.713725, 0.756863, 1)
 * id: 203 Name: plum      RGBA: (0.866667, 0.627451, 0.866667, 1)
 * id: 204 Name: powder_blue       RGBA: (0.690196, 0.878431, 0.901961, 1)
 * id: 205 Name: powderblue        RGBA: (0.690196, 0.878431, 0.901961, 1)
 * id: 206 Name: purple    RGBA: (0.501961, 0, 0.501961, 1)
 * id: 207 Name: purple_medium     RGBA: (0.576471, 0.439216, 0.858824, 1)
 * id: 208 Name: raspberry RGBA: (0.529412, 0.14902, 0.341176, 1)
 * id: 209 Name: raw_sienna        RGBA: (0.780392, 0.380392, 0.0784314, 1)
 * id: 210 Name: raw_umber RGBA: (0.45098, 0.290196, 0.0705882, 1)
 * id: 211 Name: red       RGBA: (1, 0, 0, 1)
 * id: 212 Name: rose_madder       RGBA: (0.890196, 0.211765, 0.219608, 1)
 * id: 213 Name: rosy_brown        RGBA: (0.737255, 0.560784, 0.560784, 1)
 * id: 214 Name: rosybrown RGBA: (0.737255, 0.560784, 0.560784, 1)
 * id: 215 Name: royal_blue        RGBA: (0.254902, 0.411765, 0.882353, 1)
 * id: 216 Name: royalblue RGBA: (0.254902, 0.411765, 0.882353, 1)
 * id: 217 Name: saddle_brown      RGBA: (0.545098, 0.270588, 0.0745098, 1)
 * id: 218 Name: saddlebrown       RGBA: (0.545098, 0.270588, 0.0745098, 1)
 * id: 219 Name: salmon    RGBA: (0.980392, 0.501961, 0.447059, 1)
 * id: 220 Name: sandy_brown       RGBA: (0.956863, 0.643137, 0.376471, 1)
 * id: 221 Name: sandybrown        RGBA: (0.956863, 0.643137, 0.376471, 1)
 * id: 222 Name: sap_green RGBA: (0.188235, 0.501961, 0.0784314, 1)
 * id: 223 Name: sea_green RGBA: (0.180392, 0.545098, 0.341176, 1)
 * id: 224 Name: sea_green_dark    RGBA: (0.560784, 0.737255, 0.560784, 1)
 * id: 225 Name: sea_green_light   RGBA: (0.12549, 0.698039, 0.666667, 1)
 * id: 226 Name: sea_green_medium  RGBA: (0.235294, 0.701961, 0.443137, 1)
 * id: 227 Name: seagreen  RGBA: (0.180392, 0.545098, 0.341176, 1)
 * id: 228 Name: seashell  RGBA: (1, 0.960784, 0.933333, 1)
 * id: 229 Name: sepia     RGBA: (0.368627, 0.14902, 0.0705882, 1)
 * id: 230 Name: sienna    RGBA: (0.627451, 0.321569, 0.176471, 1)
 * id: 231 Name: silver    RGBA: (0.752941, 0.752941, 0.752941, 1)
 * id: 232 Name: sky_blue  RGBA: (0.529412, 0.807843, 0.921569, 1)
 * id: 233 Name: sky_blue_deep     RGBA: (0, 0.74902, 1, 1)
 * id: 234 Name: sky_blue_light    RGBA: (0.529412, 0.807843, 0.980392, 1)
 * id: 235 Name: skyblue   RGBA: (0.529412, 0.807843, 0.921569, 1)
 * id: 236 Name: slate_blue        RGBA: (0.415686, 0.352941, 0.803922, 1)
 * id: 237 Name: slate_blue_dark   RGBA: (0.282353, 0.239216, 0.545098, 1)
 * id: 238 Name: slate_blue_light  RGBA: (0.517647, 0.439216, 1, 1)
 * id: 239 Name: slate_blue_medium RGBA: (0.482353, 0.407843, 0.933333, 1)
 * id: 240 Name: slate_grey        RGBA: (0.439216, 0.501961, 0.564706, 1)
 * id: 241 Name: slate_grey_dark   RGBA: (0.184314, 0.309804, 0.309804, 1)
 * id: 242 Name: slate_grey_light  RGBA: (0.466667, 0.533333, 0.6, 1)
 * id: 243 Name: slateblue RGBA: (0.415686, 0.352941, 0.803922, 1)
 * id: 244 Name: slategray RGBA: (0.439216, 0.501961, 0.564706, 1)
 * id: 245 Name: snow      RGBA: (1, 0.980392, 0.980392, 1)
 * id: 246 Name: spring_green      RGBA: (0, 1, 0.498039, 1)
 * id: 247 Name: spring_green_medium       RGBA: (0, 0.980392, 0.603922, 1)
 * id: 248 Name: springgreen       RGBA: (0, 1, 0.498039, 1)
 * id: 249 Name: steel_blue        RGBA: (0.27451, 0.509804, 0.705882, 1)
 * id: 250 Name: steel_blue_light  RGBA: (0.690196, 0.768627, 0.870588, 1)
 * id: 251 Name: steelblue RGBA: (0.27451, 0.509804, 0.705882, 1)
 * id: 252 Name: tan       RGBA: (0.823529, 0.705882, 0.54902, 1)
 * id: 253 Name: teal      RGBA: (0, 0.501961, 0.501961, 1)
 * id: 254 Name: terre_verte       RGBA: (0.219608, 0.368627, 0.0588235, 1)
 * id: 255 Name: thistle   RGBA: (0.847059, 0.74902, 0.847059, 1)
 * id: 256 Name: titaniuwhite    RGBA: (0.988235, 1, 0.941176, 1)
 * id: 257 Name: tomato    RGBA: (1, 0.388235, 0.278431, 1)
 * id: 258 Name: turquoise RGBA: (0.25098, 0.878431, 0.815686, 1)
 * id: 259 Name: turquoise_blue    RGBA: (0, 0.780392, 0.54902, 1)
 * id: 260 Name: turquoise_dark    RGBA: (0, 0.807843, 0.819608, 1)
 * id: 261 Name: turquoise_medium  RGBA: (0.282353, 0.819608, 0.8, 1)
 * id: 262 Name: turquoise_pale    RGBA: (0.686275, 0.933333, 0.933333, 1)
 * id: 263 Name: ultramarine       RGBA: (0.0705882, 0.0392157, 0.560784, 1)
 * id: 264 Name: ultramarine_violet        RGBA: (0.360784, 0.141176, 0.431373, 1)
 * id: 265 Name: van_dyke_brown    RGBA: (0.368627, 0.14902, 0.0196078, 1)
 * id: 266 Name: venetian_red      RGBA: (0.831373, 0.101961, 0.121569, 1)
 * id: 267 Name: violet    RGBA: (0.933333, 0.509804, 0.933333, 1)
 * id: 268 Name: violet_dark       RGBA: (0.580392, 0, 0.827451, 1)
 * id: 269 Name: violet_red        RGBA: (0.815686, 0.12549, 0.564706, 1)
 * id: 270 Name: violet_red_medium RGBA: (0.780392, 0.0823529, 0.521569, 1)
 * id: 271 Name: violet_red_pale   RGBA: (0.858824, 0.439216, 0.576471, 1)
 * id: 272 Name: viridian_light    RGBA: (0.431373, 1, 0.439216, 1)
 * id: 273 Name: wargrey RGBA: (0.501961, 0.501961, 0.411765, 1)
 * id: 274 Name: wheat     RGBA: (0.960784, 0.870588, 0.701961, 1)
 * id: 275 Name: white     RGBA: (1, 1, 1, 1)
 * id: 276 Name: white_smoke       RGBA: (0.960784, 0.960784, 0.960784, 1)
 * id: 277 Name: whitesmoke        RGBA: (0.960784, 0.960784, 0.960784, 1)
 * id: 278 Name: yellow    RGBA: (1, 1, 0, 1)
 * id: 279 Name: yellow_green      RGBA: (0.603922, 0.803922, 0.196078, 1)
 * id: 280 Name: yellow_light      RGBA: (1, 1, 0.878431, 1)
 * id: 281 Name: yellow_ochre      RGBA: (0.890196, 0.509804, 0.0901961, 1)
 * id: 282 Name: yellowgreen       RGBA: (0.603922, 0.803922, 0.196078, 1)
 * id: 283 Name: zinc_white        RGBA: (0.988235, 0.968627, 1, 1) 
*/

class ABSTRACTDATA_EXPORT QvtkImageLabel2 : public QvtkImage
{
	Q_OBJECT;
	Q_VTK_DATAH(
		QvtkImageLabel2,
		Q_VTK_KEY(Label)
		//Q_VTK_KEY(LabelName)
		Q_VTK_KEY(LabelId)
		Q_VTK_KEY(LabelRGBA)
	);


public:

	QvtkImageLabel2();
	virtual ~QvtkImageLabel2() override;
	virtual void printSelf() const override;

	virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
	virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;

	virtual void addReference(QvtkAbstractProp* prop) override;

	virtual vtkAlgorithmOutput* getOutputPort() const;
	virtual vtkLookupTable* getLookupTable() const { return this->lookupTable; }


	virtual const double* getColor(int id) const;
	virtual void getColor(int id, double rgba[4]) const;
	virtual const double* getColor(QString labelName) const;
	virtual void getColor(QString labelName, double rgba[4]) const;

	const QMap<QString, int> getNameToValueMap();

	/**
	 * QT_NAMESPACE::QString getColorName
	 *
	 * @brief 
	 *
	 * Return the color representing input value
	 *
	 * @param int  
	 * @return QString
	 */
	virtual QString getColorName(int val) const;
	
	/**
	 * int getLabelIdFromName
	 *
	 * @brief 
	 *
	 * Return the value represented by the input qstring
	 *
	 * @param QString labelName 
	 * @return int
	 */
	virtual int getLabelIdFromName(QString labelName) const;

public slots:
	/**
	 * void setColor
	 *
	 * @brief 
	 *
	 * These will be depicted soon.
	 *
	 * @param int id 
	 * @param const double * rgba 
	 * @return void
	 */
	virtual void setColor(int id, const double* rgba) { this->setColor(id, rgba[0], rgba[1], rgba[2], rgba[3]); }
	virtual void setColor(int id, double r, double g, double b, double a);
	virtual void setColor(QString labelName, const double* rgba) { this->setColor(labelName, rgba[0], rgba[1], rgba[2], rgba[3]); }
	virtual void setColor(QString labelName, double r, double g, double b, double a);


	/**
	 * void setColor
	 *
	 * @brief 
	 *
	 * Set current color of a label or add a new color.
	 *
	 * @param QString labelName 
	 * @param int value 
	 * @param double[4]  
	 * @return void
	 */
	virtual void setColor(QString labelName, int value, double[4]);
	virtual void setColor(QString labelName, int value, double r, double g, double b, double a);


	virtual void initializeLabel(QvtkImage* image, int type = 3);

protected:

	template<typename ScalarType>
	static void zeroImage(vtkImageData* image);

	virtual Data* newInstance() const override { return new QvtkImageLabel2; }

	/**
	 * void rebuildLookupTable
	 *
	 * @brief 
	 *
	 * Build look up table from the two QMap
	 *
	 * @return void
	 */
	virtual void rebuildLookupTable();

	/**
	* void writeLabel
	*
	* @brief
	*
	* Same as read/writeXML
	*
	* @param QDomElement & xml
	* @return void
	*/
	virtual void readLabel(const QDomElement& xml);
	
	/**
	 * void writeLabel
	 *
	 * @brief 
	 *
	 * Same as read/writeXML
	 *
	 * @param QDomElement & xml 
	 * @return void
	 */
	virtual void writeLabel(QDomElement& xml) const;

private:

	QMap<int, double*> valueToColorMap;
	QMap<QString, int> nameToValueMap;
	vtkLookupTable* lookupTable;
	vtkSmartPointer<vtkPassThroughFilter> outputfilter;
};


#endif // !__Qvtk_IMAGE_LABEL_H__
