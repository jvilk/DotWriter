/**
 * GEnums.h
 *
 * Stores all of the support enums used in the graph library.
 *
 * DO NOT CHANGE THE ORDER OF THE ENUMS WITHOUT CHANGING THE STRINGS STORED IN
 * GEnums.cpp.
 *
 * All enums have a DEFAULT value. This is interpreted to mean, "Do not
 * explicitly specify in the DOT file."
 *
 * All enums also have a COUNT value which is used for bounds checking. This
 * must be the last item in the enum.
 * 
 * Author: John Vilk
 */

#ifndef DOTWRITER_ENUMS_H_
#define DOTWRITER_ENUMS_H_

namespace DotWriter {

/** We encapsulate each enum inside a struct to prevent name collisions. **/

struct EdgeArrowTypeName {
  enum e {
    DEFAULT,
    NORMAL,
    INV,
    DOT,
    INVDOT,
    ODOT,
    INVODOT,
    NONE,
    TEE,
    EMPTY,
    INVEMPTY,
    DIAMOND,
    ODIAMOND,
    EDIAMOND,
    CROW,
    BOX,
    OBOX,
    OPEN,
    HALFOPEN,
    VEE,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(EdgeArrowTypeName::e);
};

struct EdgeStyle {
  enum e {
    DEFAULT,
    DASHED,
    DOTTED,
    SOLID,
    INVIS,
    BOLD,
    TAPERED,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(EdgeStyle::e val);
};

struct DirType {
  enum e {
    DEFAULT,
    FORWARD,
    BACK,
    BOTH,
    NONE,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(DirType::e val);
};

struct NodeStyle {
  enum e {
    DEFAULT,
    DASHED,
    DOTTED,
    SOLID,
    INVIS,
    BOLD,
    FILLED,
    DIAGONALS,
    ROUNDED,
    RADIAL,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(NodeStyle::e val);
};

struct NodeShape {
  enum e {
    DEFAULT,
    BOX,
    POLYGON,
    ELLIPSE,
    OVAL,
    CIRCLE,
    POINT,
    EGG,
    TRIANGLE,
    PLAINTEXT,
    DIAMOND,
    TRAPEZIUM,
    PARALLELOGRAM,
    HOUSE,
    PENTAGON,
    HEXAGON,
    SEPTAGON,
    OCTAGON,
    DOUBLECIRCLE,
    DOUBLEOCTAGON,
    TRIPLEOCTAGON,
    INVTRIANGLE,
    INVTRAPEZIUM,
    INVHOUSE,
    MDIAMOND,
    MSQUARE,
    MCIRCLE,
    RECT,
    RECTANGLE,
    SQUARE,
    NONE,
    NOTE,
    TAB,
    FOLDER,
    BOX3D,
    COMPONENT,
    COUNT
  };

private:
    static const char* strings[];

public:
  static const char* ToString(NodeShape::e val);
};

struct Charset {
  enum e {
    DEFAULT,
    UTF8,
    LATIN1,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Charset::e val);
};

struct OutputMode {
  enum e {
    DEFAULT,
    BREADTHFIRST,
    NODESFIRST,
    EDGESFIRST,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(OutputMode::e val);
};

struct ClusterMode {
  enum e {
    DEFAULT,
    LOCAL,
    GLOBAL,
    NONE,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(ClusterMode::e val);
};

struct LabelLoc {
  enum e {
    DEFAULT,
    T,
    B,
    C,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(LabelLoc::e val);
};

struct PageDir {
  enum e {
    DEFAULT,
    BL, 
    BR,
    TL,
    TR,
    RB,
    RT,
    LB,
    LT,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(PageDir::e val);
};

struct QuadType {
  enum e {
    DEFAULT,
    NORMAL,
    FAST,
    NONE,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(QuadType::e val);
};

struct RankType {
  enum e {
    DEFAULT,
    SAME,
    MIN,
    SOURCE,
    MAX,
    SINK,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(RankType::e val);
};

struct RankDir {
  enum e {
    DEFAULT,
    TB,
    LR,
    BT,
    RL,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(RankDir::e val);
};

struct DirEdgeConstraints {
  enum e {
    DEFAULT,
    TRUE,
    HIER,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(DirEdgeConstraints::e val);
};

struct CompassPoint {
  enum e {
    DEFAULT,
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW,
    C,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(CompassPoint::e val);
};

struct Model {
  enum e {
    DEFAULT,
    CIRCUIT,
    SUBSET,
    MDS,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Model::e val);
};

struct Ordering {
  enum e {
    DEFAULT,
    OUT,
    IN,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Ordering::e val);
};

struct Mode {
  enum e {
    DEFAULT,
    MAJOR,
    KK,
    HIER,
    IPSEP,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Mode::e val);
};

struct Justification {
  enum e {
    DEFAULT,
    LEFT,
    RIGHT,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Justification::e val);
};

struct Ratio {
  enum e {
    DEFAULT,
    FILL,
    COMPRESS,
    EXPAND,
    AUTO,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Ratio::e val);
};

struct SmoothType {
  enum e {
    DEFAULT,
    NONE,
    AVG_DIST,
    GRAPH_DIST,
    POWER_DIST,
    RNG,
    SPRING,
    TRIANGLE,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(SmoothType::e val);
};

struct SplineType {
  enum e {
    DEFAULT,
    LINE,
    SPLINE,
    POLYLINE,
    ORTHO,
    COMPOUND,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(SplineType::e val);
};

struct ImageScaleType {
  enum e {
    DEFAULT,
    WIDTH,
    HEIGHT,
    BOTH,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(ImageScaleType::e val);
};

/**
 * Represents valid colors in DOT files.
 *
 * What have I done?
 *
 * http://www.graphviz.org/content/color-names
 */
struct Color {
  enum e {
    DEFAULT,
    ALICEBLUE,
    ANTIQUEWHITE,
    ANTIQUEWHITE1,
    ANTIQUEWHITE2,
    ANTIQUEWHITE3,
    ANTIQUEWHITE4,
    AQUAMARINE,
    AQUAMARINE1,
    AQUAMARINE2,
    AQUAMARINE3,
    AQUAMARINE4,
    AZURE,
    AZURE1,
    AZURE2,
    AZURE3,
    AZURE4,
    BEIGE,
    BISQUE,
    BISQUE1,
    BISQUE2,
    BISQUE3,
    BISQUE4,
    BLACK,
    BLANCHEDALMOND,
    BLUE,
    BLUE1,
    BLUE2,
    BLUE3,
    BLUE4,
    BLUEVIOLET,
    BROWN,
    BROWN1,
    BROWN2,
    BROWN3,
    BROWN4,
    BURLYWOOD,
    BURLYWOOD1,
    BURLYWOOD2,
    BURLYWOOD3,
    BURLYWOOD4,
    CADETBLUE,
    CADETBLUE1,
    CADETBLUE2,
    CADETBLUE3,
    CADETBLUE4,
    CHARTREUSE,
    CHARTREUSE1,
    CHARTREUSE2,
    CHARTREUSE3,
    CHARTREUSE4,
    CHOCOLATE,
    CHOCOLATE1,
    CHOCOLATE2,
    CHOCOLATE3,
    CHOCOLATE4,
    CORAL,
    CORAL1,
    CORAL2,
    CORAL3,
    CORAL4,
    CORNFLOWERBLUE,
    CORNSILK,
    CORNSILK1,
    CORNSILK2,
    CORNSILK3,
    CORNSILK4,
    CRIMSON,
    CYAN,
    CYAN1,
    CYAN2,
    CYAN3,
    CYAN4,
    DARKGOLDENROD,
    DARKGOLDENROD1,
    DARKGOLDENROD2,
    DARKGOLDENROD3,
    DARKGOLDENROD4,
    DARKGREEN,
    DARKKHAKI,
    DARKOLIVEGREEN,
    DARKOLIVEGREEN1,
    DARKOLIVEGREEN2,
    DARKOLIVEGREEN3,
    DARKOLIVEGREEN4,
    DARKORANGE,
    DARKORANGE1,
    DARKORANGE2,
    DARKORANGE3,
    DARKORANGE4,
    DARKORCHID,
    DARKORCHID1,
    DARKORCHID2,
    DARKORCHID3,
    DARKORCHID4,
    DARKSALMON,
    DARKSEAGREEN,
    DARKSEAGREEN1,
    DARKSEAGREEN2,
    DARKSEAGREEN3,
    DARKSEAGREEN4,
    DARKSLATEBLUE,
    DARKSLATEGRAY,
    DARKSLATEGRAY1,
    DARKSLATEGRAY2,
    DARKSLATEGRAY3,
    DARKSLATEGRAY4,
    DARKSLATEGREY,
    DARKTURQUOISE,
    DARKVIOLET,
    DEEPPINK,
    DEEPPINK1,
    DEEPPINK2,
    DEEPPINK3,
    DEEPPINK4,
    DEEPSKYBLUE,
    DEEPSKYBLUE1,
    DEEPSKYBLUE2,
    DEEPSKYBLUE3,
    DEEPSKYBLUE4,
    DIMGRAY,
    DIMGREY,
    DODGERBLUE,
    DODGERBLUE1,
    DODGERBLUE2,
    DODGERBLUE3,
    DODGERBLUE4,
    FIREBRICK,
    FIREBRICK1,
    FIREBRICK2,
    FIREBRICK3,
    FIREBRICK4,
    FLORALWHITE,
    FORESTGREEN,
    GAINSBORO,
    GHOSTWHITE,
    GOLD,
    GOLD1,
    GOLD2,
    GOLD3,
    GOLD4,
    GOLDENROD,
    GOLDENROD1,
    GOLDENROD2,
    GOLDENROD3,
    GOLDENROD4,
    GRAY,
    GRAY0,
    GRAY1,
    GRAY10,
    GRAY100,
    GRAY11,
    GRAY12,
    GRAY13,
    GRAY14,
    GRAY15,
    GRAY16,
    GRAY17,
    GRAY18,
    GRAY19,
    GRAY2,
    GRAY20,
    GRAY21,
    GRAY22,
    GRAY23,
    GRAY24,
    GRAY25,
    GRAY26,
    GRAY27,
    GRAY28,
    GRAY29,
    GRAY3,
    GRAY30,
    GRAY31,
    GRAY32,
    GRAY33,
    GRAY34,
    GRAY35,
    GRAY36,
    GRAY37,
    GRAY38,
    GRAY39,
    GRAY4,
    GRAY40,
    GRAY41,
    GRAY42,
    GRAY43,
    GRAY44,
    GRAY45,
    GRAY46,
    GRAY47,
    GRAY48,
    GRAY49,
    GRAY5,
    GRAY50,
    GRAY51,
    GRAY52,
    GRAY53,
    GRAY54,
    GRAY55,
    GRAY56,
    GRAY57,
    GRAY58,
    GRAY59,
    GRAY6,
    GRAY60,
    GRAY61,
    GRAY62,
    GRAY63,
    GRAY64,
    GRAY65,
    GRAY66,
    GRAY67,
    GRAY68,
    GRAY69,
    GRAY7,
    GRAY70,
    GRAY71,
    GRAY72,
    GRAY73,
    GRAY74,
    GRAY75,
    GRAY76,
    GRAY77,
    GRAY78,
    GRAY79,
    GRAY8,
    GRAY80,
    GRAY81,
    GRAY82,
    GRAY83,
    GRAY84,
    GRAY85,
    GRAY86,
    GRAY87,
    GRAY88,
    GRAY89,
    GRAY9,
    GRAY90,
    GRAY91,
    GRAY92,
    GRAY93,
    GRAY94,
    GRAY95,
    GRAY96,
    GRAY97,
    GRAY98,
    GRAY99,
    GREEN,
    GREEN1,
    GREEN2,
    GREEN3,
    GREEN4,
    GREENYELLOW,
    GREY,
    GREY0,
    GREY1,
    GREY10,
    GREY100,
    GREY11,
    GREY12,
    GREY13,
    GREY14,
    GREY15,
    GREY16,
    GREY17,
    GREY18,
    GREY19,
    GREY2,
    GREY20,
    GREY21,
    GREY22,
    GREY23,
    GREY24,
    GREY25,
    GREY26,
    GREY27,
    GREY28,
    GREY29,
    GREY3,
    GREY30,
    GREY31,
    GREY32,
    GREY33,
    GREY34,
    GREY35,
    GREY36,
    GREY37,
    GREY38,
    GREY39,
    GREY4,
    GREY40,
    GREY41,
    GREY42,
    GREY43,
    GREY44,
    GREY45,
    GREY46,
    GREY47,
    GREY48,
    GREY49,
    GREY5,
    GREY50,
    GREY51,
    GREY52,
    GREY53,
    GREY54,
    GREY55,
    GREY56,
    GREY57,
    GREY58,
    GREY59,
    GREY6,
    GREY60,
    GREY61,
    GREY62,
    GREY63,
    GREY64,
    GREY65,
    GREY66,
    GREY67,
    GREY68,
    GREY69,
    GREY7,
    GREY70,
    GREY71,
    GREY72,
    GREY73,
    GREY74,
    GREY75,
    GREY76,
    GREY77,
    GREY78,
    GREY79,
    GREY8,
    GREY80,
    GREY81,
    GREY82,
    GREY83,
    GREY84,
    GREY85,
    GREY86,
    GREY87,
    GREY88,
    GREY89,
    GREY9,
    GREY90,
    GREY91,
    GREY92,
    GREY93,
    GREY94,
    GREY95,
    GREY96,
    GREY97,
    GREY98,
    GREY99,
    HONEYDEW,
    HONEYDEW1,
    HONEYDEW2,
    HONEYDEW3,
    HONEYDEW4,
    HOTPINK,
    HOTPINK1,
    HOTPINK2,
    HOTPINK3,
    HOTPINK4,
    INDIANRED,
    INDIANRED1,
    INDIANRED2,
    INDIANRED3,
    INDIANRED4,
    INDIGO,
    INVIS,
    IVORY,
    IVORY1,
    IVORY2,
    IVORY3,
    IVORY4,
    KHAKI,
    KHAKI1,
    KHAKI2,
    KHAKI3,
    KHAKI4,
    LAVENDER,
    LAVENDERBLUSH,
    LAVENDERBLUSH1,
    LAVENDERBLUSH2,
    LAVENDERBLUSH3,
    LAVENDERBLUSH4,
    LAWNGREEN,
    LEMONCHIFFON,
    LEMONCHIFFON1,
    LEMONCHIFFON2,
    LEMONCHIFFON3,
    LEMONCHIFFON4,
    LIGHTBLUE,
    LIGHTBLUE1,
    LIGHTBLUE2,
    LIGHTBLUE3,
    LIGHTBLUE4,
    LIGHTCORAL,
    LIGHTCYAN,
    LIGHTCYAN1,
    LIGHTCYAN2,
    LIGHTCYAN3,
    LIGHTCYAN4,
    LIGHTGOLDENROD,
    LIGHTGOLDENROD1,
    LIGHTGOLDENROD2,
    LIGHTGOLDENROD3,
    LIGHTGOLDENROD4,
    LIGHTGOLDENRODYELLOW,
    LIGHTGRAY,
    LIGHTGREY,
    LIGHTPINK,
    LIGHTPINK1,
    LIGHTPINK2,
    LIGHTPINK3,
    LIGHTPINK4,
    LIGHTSALMON,
    LIGHTSALMON1,
    LIGHTSALMON2,
    LIGHTSALMON3,
    LIGHTSALMON4,
    LIGHTSEAGREEN,
    LIGHTSKYBLUE,
    LIGHTSKYBLUE1,
    LIGHTSKYBLUE2,
    LIGHTSKYBLUE3,
    LIGHTSKYBLUE4,
    LIGHTSLATEBLUE,
    LIGHTSLATEGRAY,
    LIGHTSLATEGREY,
    LIGHTSTEELBLUE,
    LIGHTSTEELBLUE1,
    LIGHTSTEELBLUE2,
    LIGHTSTEELBLUE3,
    LIGHTSTEELBLUE4,
    LIGHTYELLOW,
    LIGHTYELLOW1,
    LIGHTYELLOW2,
    LIGHTYELLOW3,
    LIGHTYELLOW4,
    LIMEGREEN,
    LINEN,
    MAGENTA,
    MAGENTA1,
    MAGENTA2,
    MAGENTA3,
    MAGENTA4,
    MAROON,
    MAROON1,
    MAROON2,
    MAROON3,
    MAROON4,
    MEDIUMAQUAMARINE,
    MEDIUMBLUE,
    MEDIUMORCHID,
    MEDIUMORCHID1,
    MEDIUMORCHID2,
    MEDIUMORCHID3,
    MEDIUMORCHID4,
    MEDIUMPURPLE,
    MEDIUMPURPLE1,
    MEDIUMPURPLE2,
    MEDIUMPURPLE3,
    MEDIUMPURPLE4,
    MEDIUMSEAGREEN,
    MEDIUMSLATEBLUE,
    MEDIUMSPRINGGREEN,
    MEDIUMTURQUOISE,
    MEDIUMVIOLETRED,
    MIDNIGHTBLUE,
    MINTCREAM,
    MISTYROSE,
    MISTYROSE1,
    MISTYROSE2,
    MISTYROSE3,
    MISTYROSE4,
    MOCCASIN,
    NAVAJOWHITE,
    NAVAJOWHITE1,
    NAVAJOWHITE2,
    NAVAJOWHITE3,
    NAVAJOWHITE4,
    NAVY,
    NAVYBLUE,
    NONE,
    OLDLACE,
    OLIVEDRAB,
    OLIVEDRAB1,
    OLIVEDRAB2,
    OLIVEDRAB3,
    OLIVEDRAB4,
    ORANGE,
    ORANGE1,
    ORANGE2,
    ORANGE3,
    ORANGE4,
    ORANGERED,
    ORANGERED1,
    ORANGERED2,
    ORANGERED3,
    ORANGERED4,
    ORCHID,
    ORCHID1,
    ORCHID2,
    ORCHID3,
    ORCHID4,
    PALEGOLDENROD,
    PALEGREEN,
    PALEGREEN1,
    PALEGREEN2,
    PALEGREEN3,
    PALEGREEN4,
    PALETURQUOISE,
    PALETURQUOISE1,
    PALETURQUOISE2,
    PALETURQUOISE3,
    PALETURQUOISE4,
    PALEVIOLETRED,
    PALEVIOLETRED1,
    PALEVIOLETRED2,
    PALEVIOLETRED3,
    PALEVIOLETRED4,
    PAPAYAWHIP,
    PEACHPUFF,
    PEACHPUFF1,
    PEACHPUFF2,
    PEACHPUFF3,
    PEACHPUFF4,
    PERU,
    PINK,
    PINK1,
    PINK2,
    PINK3,
    PINK4,
    PLUM,
    PLUM1,
    PLUM2,
    PLUM3,
    PLUM4,
    POWDERBLUE,
    PURPLE,
    PURPLE1,
    PURPLE2,
    PURPLE3,
    PURPLE4,
    RED,
    RED1,
    RED2,
    RED3,
    RED4,
    ROSYBROWN,
    ROSYBROWN1,
    ROSYBROWN2,
    ROSYBROWN3,
    ROSYBROWN4,
    ROYALBLUE,
    ROYALBLUE1,
    ROYALBLUE2,
    ROYALBLUE3,
    ROYALBLUE4,
    SADDLEBROWN,
    SALMON,
    SALMON1,
    SALMON2,
    SALMON3,
    SALMON4,
    SANDYBROWN,
    SEAGREEN,
    SEAGREEN1,
    SEAGREEN2,
    SEAGREEN3,
    SEAGREEN4,
    SEASHELL,
    SEASHELL1,
    SEASHELL2,
    SEASHELL3,
    SEASHELL4,
    SIENNA,
    SIENNA1,
    SIENNA2,
    SIENNA3,
    SIENNA4,
    SKYBLUE,
    SKYBLUE1,
    SKYBLUE2,
    SKYBLUE3,
    SKYBLUE4,
    SLATEBLUE,
    SLATEBLUE1,
    SLATEBLUE2,
    SLATEBLUE3,
    SLATEBLUE4,
    SLATEGRAY,
    SLATEGRAY1,
    SLATEGRAY2,
    SLATEGRAY3,
    SLATEGRAY4,
    SLATEGREY,
    SNOW,
    SNOW1,
    SNOW2,
    SNOW3,
    SNOW4,
    SPRINGGREEN,
    SPRINGGREEN1,
    SPRINGGREEN2,
    SPRINGGREEN3,
    SPRINGGREEN4,
    STEELBLUE,
    STEELBLUE1,
    STEELBLUE2,
    STEELBLUE3,
    STEELBLUE4,
    TAN,
    TAN1,
    TAN2,
    TAN3,
    TAN4,
    THISTLE,
    THISTLE1,
    THISTLE2,
    THISTLE3,
    THISTLE4,
    TOMATO,
    TOMATO1,
    TOMATO2,
    TOMATO3,
    TOMATO4,
    TRANSPARENT,
    TURQUOISE,
    TURQUOISE1,
    TURQUOISE2,
    TURQUOISE3,
    TURQUOISE4,
    VIOLET,
    VIOLETRED,
    VIOLETRED1,
    VIOLETRED2,
    VIOLETRED3,
    VIOLETRED4,
    WHEAT,
    WHEAT1,
    WHEAT2,
    WHEAT3,
    WHEAT4,
    WHITE,
    WHITESMOKE,
    YELLOW,
    YELLOW1,
    YELLOW2,
    YELLOW3,
    YELLOW4,
    YELLOWGREEN,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(Color::e val);
};

struct AttributeType {
  enum e {
    DAMPING,
    K,
    URL,
    AREA,
    ARROWHEAD,
    ARROWSIZE,
    ARROWTAIL,
    ASPECT,
    BB,
    BGCOLOR,
    CENTER,
    CHARSET,
    CLUSTERRANK,
    COLOR,
    COLORSCHEME,
    COMMENT,
    COMPOUND,
    CONCENTRATE,
    CONSTRAINT,
    DECORATE,
    DEFAULTDIST,
    DIM,
    DIMEN,
    DIR,
    DIREDGECONSTRAINTS,
    DISTORTION,
    DPI,
    EDGEURL,
    EDGEHREF,
    EDGETARGET,
    EDGETOOLTIP,
    EPSILON,
    ESEP,
    FILLCOLOR,
    FIXEDSIZE,
    FONTCOLOR,
    FONTNAME,
    FONTNAMES,
    FONTPATH,
    FONTSIZE,
    FORCELABELS,
    GRADIENTANGLE,
    GROUP,
    HEADURL,
    HEADCLIP,
    HEADHREF,
    HEADLABEL,
    HEADPORT,
    HEADTARGET,
    HEADTOOLTIP,
    HEIGHT,
    HREF,
    ID,
    IMAGE,
    IMAGEPATH,
    IMAGESCALE,
    LABEL,
    LABELURL,
    LABEL_SCHEME,
    LABELANGLE,
    LABELDISTANCE,
    LABELFLOAT,
    LABELFONTCOLOR,
    LABELFONTNAME,
    LABELFONTSIZE,
    LABELHREF,
    LABELJUST,
    LABELLOC,
    LABELTARGET,
    LABELTOOLTIP,
    LANDSCAPE,
    LAYER,
    LAYERS,
    LAYERSELECT,
    LAYERSEP,
    LAYOUT,
    LEN,
    LEVELS,
    LEVELSGAP,
    LHEAD,
    LHEIGHT,
    LP,
    LTAIL,
    LWIDTH,
    MARGIN,
    MAXITER,
    MCLIMIT,
    MINDIST,
    MINLEN,
    MODE,
    MODEL,
    MOSEK,
    NODESEP,
    NOJUSTIFY,
    NORMALIZE,
    NSLIMIT,
    NSLIMIT1,
    ORDERING,
    ORIENTATION,
    OUTPUTORDER,
    OVERLAP,
    OVERLAP_SCALING,
    PACK,
    PACKMODE,
    PAD,
    PAGE,
    PAGEDIR,
    PENCOLOR,
    PENWIDTH,
    PERIPHERIES,
    PIN,
    POS,
    QUADTREE,
    QUANTUM,
    RANK,
    RANKDIR,
    RANKSEP,
    RATIO,
    RECTS,
    REGULAR,
    REMINCROSS,
    REPULSIVEFORCE,
    RESOLUTION,
    ROOT,
    ROTATE,
    ROTATION,
    SAMEHEAD,
    SAMETAIL,
    SAMPLEPOINTS,
    SCALE,
    SEARCHSIZE,
    SEP,
    SHAPE,
    SHOWBOXES,
    SIDES,
    SIZE,
    SKEW,
    SMOOTHING,
    SORTV,
    SPLINES,
    START,
    STYLE,
    STYLESHEET,
    TAILURL,
    TAILCLIP,
    TAILHREF,
    TAILLABEL,
    TAILPORT,
    TAILTARGET,
    TAILTOOLTIP,
    TARGET,
    TOOLTIP,
    TRUECOLOR,
    VERTICES,
    VIEWPORT,
    VORO_MARGIN,
    WEIGHT,
    WIDTH,
    XLABEL,
    COUNT
  };

private:
  static const char* strings[];

public:
  static const char* ToString(AttributeType::e val);
};

}  // namespace DotWriter
#endif
