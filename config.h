/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char buttonbar[]       = " ";
static const char *fonts[]          = {"TerminessTTF Nerd Font:size=12"};
static const char dmenufont[]       =  "TerminessTTF Nerd Font:size=12:";
static const char col_gray1[]       = "#2e3440";
static const char col_gray2[]       = "#3b4252";
static const char col_gray3[]       = "#4c566a";
static const char col_gray4[]       = "#d8dee9";
static const char col_cyan[]        = "#81a1c1";
static const char *colors[][3]      = {
	[SchemeNorm] = { "#d8dee9", "#2e3440", "#434c5e" },
	[SchemeSel]  = { "#f8f8f2", "#bd93f9" ,  "#8fbcbb"  },
	[SchemeStatus]  = { col_gray1,"#5E81AC",  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_gray4, col_cyan,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { col_gray3, col_gray1,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { col_cyan, "#434c5e",  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { col_gray3, col_gray1,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { " "," "," ", " ", " "," ", " ",  " ", " "};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,          -1,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,          -1,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         1,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﬿",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
	{ "頻 ",      centeredmaster },
	{ "恵 ",      centeredfloatingmaster },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

void swaptags(const Arg *arg);

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ Mod1Mask|ShiftMask,           KEY,      swaptags,       {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "60x17",NULL };
static const char *bravecmd[] = { "brave", NULL };
static const char *chromiumcmd[] = { "chromium", NULL };
static const char *incognitobravecmd[] = { "brave", "--incognito",NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MODKEY,		                XK_e,      spawn,          SHCMD("emacs") },
    { MODKEY,                       XK_a,      spawn,          SHCMD("clipmenu") },
    { MODKEY,                       XK_F3,     spawn,          SHCMD(" amixer -D pulse sset Master 5%+ ;refbar.sh")},
    { MODKEY,                       XK_F2,     spawn,          SHCMD(" amixer -D pulse sset Master 5%-;refbar.sh")},
    { MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("slock")},
    { 0,                            XK_Print,  spawn,          SHCMD(" flameshot full -p $HOME/Images/screenshots")},
    { Mod1Mask,                     XK_Print,  spawn,          SHCMD(" flameshot gui")},
    { Mod1Mask,                     XK_c,      spawn,          SHCMD("/home/gabriel/.local/bin/show_configs") },
    { Mod1Mask,                     XK_l,      spawn,          SHCMD("/home/gabriel/.local/bin/show_books") },
    { MODKEY,                     XK_F12,    spawn,          SHCMD("xbacklight -inc 1 ; refbar.sh")},
    { MODKEY,                     XK_F11,    spawn,          SHCMD("xbacklight -dec 1 ; refbar.sh")},
    { Mod1Mask,                     XK_t,      spawn,          SHCMD("telegram-desktop") },
    { Mod1Mask|ShiftMask,           XK_q,      spawn,          SHCMD("shutdown -h now")},
    { MODKEY,                       XK_F5,     spawn,          SHCMD("playerctl play-pause") },
    { MODKEY,                       XK_F4,     spawn,          SHCMD("playerctl previous") },
    { MODKEY,                       XK_F6,     spawn,          SHCMD("playerctl next") },
    { MODKEY,                       XK_b,      spawn,          SHCMD("brave") },
    { MODKEY,                       XK_f,      spawn,          SHCMD("firefox") },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("pcmanfm") },
    { MODKEY|ShiftMask,             XK_b,      spawn,          {.v = incognitobravecmd} },
    { MODKEY,                       XK_c,      spawn,          {.v = chromiumcmd} },
	{ Mod1Mask,                     XK_space,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ Mod1Mask|ShiftMask,           XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	/* { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[5]} },
	/* { MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} }, */
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_bracketleft,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_bracketright, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_bracketleft,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_bracketright, tagmon,         {.i = +1 } },
    { Mod1Mask|ShiftMask,             XK_minus,  setgaps,        {.i = -1 } },
	{ Mod1Mask|ShiftMask,             XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,           XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY|ShiftMask,           XK_minus,  setgaps,        {.i = 10  } },
	{ ControlMask|ShiftMask,        XK_bracketleft,  tagswapmon,     {.i = +1 } },
	{ ControlMask|ShiftMask,        XK_bracketright, tagswapmon,     {.i = -1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkButton,    		0,      		Button1,    	spawn,	    	SHCMD("xmenu.sh") },
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod1Mask,       Button1,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
swaptags(const Arg *arg)
{
	unsigned int newtag = arg->ui & TAGMASK;
	unsigned int curtag = selmon->tagset[selmon->seltags];

	if (newtag == curtag || !curtag || (curtag & (curtag-1)))
		return;

	for (Client *c = selmon->clients; c != NULL; c = c->next) {
		if((c->tags & newtag) || (c->tags & curtag))
			c->tags ^= curtag ^ newtag;

		if(!c->tags) c->tags = newtag;
	}

	selmon->tagset[selmon->seltags] = newtag;

	focus(NULL);
	arrange(selmon);
}
