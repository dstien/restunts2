#pragma once

#include <stdint.h>

// clib
#define printf  _printf
#define strcat  _strcat
#define strcmp  _strcmp
#define stricmp _stricmp
#define strcpy  _strcpy
#define strlen  _strlen
#define strrchr _strrchr

extern void __cdecl _printf(const char*, ...);
extern char* __cdecl _strcat(char*, const char*);
extern int __cdecl _strcmp(const char*, const char*);
extern int __cdecl _stricmp(const char*, const char*);
extern char* __cdecl _strcpy(char*, const char*);
extern int __cdecl _strlen(const char*);
extern char* __cdecl _strrchr(const char*, int);

// Stunts types
#pragma pack(push, 1)
struct VECTOR {
    int16_t x, y, z;
};

struct VECTORLONG {
    int32_t lx, ly, lz;
};

struct CARSTATE {
    struct VECTORLONG car_posWorld1;
    struct VECTORLONG car_posWorld2;
    struct VECTOR car_rotate;
    int16_t car_pseudoGravity;
    int16_t car_steeringAngle;
    int16_t car_currpm;
    int16_t car_lastrpm;
    int16_t car_idlerpm2;
    int16_t car_speeddiff;
    uint16_t car_speed;
    uint16_t car_speed2;
    uint16_t car_lastspeed;
    uint16_t car_gearratio;
    uint16_t car_gearratioshr8;
    int16_t car_knob_x;
    int16_t car_36MwhlAngle;
    int16_t car_knob_y;
    int16_t car_knob_x2;
    int16_t car_knob_y2;
    int16_t car_angle_z;
    int16_t car_40MfrontWhlAngle;
    int16_t field_42;
    int16_t car_demandedGrip;
    int16_t car_surfacegrip_sum;
    int16_t field_48;
    int16_t car_trackdata3_index;
    int16_t car_rc1[4];
    int16_t car_rc2[4];
    int16_t car_rc3[4];
    int16_t car_rc4[4];
    int16_t car_rc5[4];
    struct VECTOR car_whlWorldCrds1[4];
    struct VECTOR car_whlWorldCrds2[4];
    struct VECTOR car_vec_unk3;
    struct VECTOR car_vec_unk4;
    struct VECTOR car_vec_unk5;
    int16_t field_B6;
    int16_t field_B8;
    int16_t field_BA;
    char car_is_braking;
    char car_is_accelerating;
    char car_current_gear;
    char car_sumSurfFrontWheels;
    char car_sumSurfRearWheels;
    char car_sumSurfAllWheels;
    char car_surfaceWhl[4];
    char car_engineLimiterTimer;
    char car_slidingFlag;
    char field_C8;
    char car_crashBmpFlag;
    char car_changing_gear;
    char car_fpsmul2;
    char car_transmission;
    char field_CD;
    char field_CE;
    char field_CF;
};

struct GAMESTATE {
    int32_t game_longs1[24];
    int32_t game_longs2[24];
    int32_t game_longs3[24];
    struct VECTOR game_vec1[2];
    struct VECTOR game_vec3;
    struct VECTOR game_vec4;
    int16_t game_frame_in_sec;
    int16_t game_frames_per_sec;
    int32_t game_travDist;
    int16_t game_frame;
    int16_t game_total_finish;
    int16_t field_144;
    int16_t game_pEndFrame;
    int16_t game_oEndFrame;
    int16_t game_penalty;
    uint16_t game_impactSpeed;
    uint16_t game_topSpeed;
    int16_t game_jumpCount;
    struct CARSTATE playerstate;
    struct CARSTATE opponentstate;
    int16_t field_2F2;
    int16_t field_2F4;
    int16_t game_startcol;
    int16_t game_startcol2;
    int16_t game_startrow;
    int16_t game_startrow2;
    int16_t field_2FE[24];
    int16_t field_32E[24];
    int16_t field_35E[24];
    int16_t field_38E[24];
    char field_3BE[48];
    char kevinseed[6];
    char field_3F4;
    char game_inputmode;
    char game_3F6autoLoadEvalFlag;
    char field_3F7[2];
    char field_3F9;
    char field_3FA[48];
    char field_42A;
    char field_42B[24];
    char field_443[24];
    char field_45B;
    char field_45C;
    char field_45D;
    char field_45E;
    char field_45F;
};

struct GAMEINFO {
    char game_playercarid[4];
    char game_playermaterial;
    char game_playertransmission;
    char game_opponenttype;
    char game_opponentcarid[4];
    char game_opponentmaterial;
    char game_opponenttransmission;
    char game_trackname[9];
    uint16_t game_framespersec;
    uint16_t game_recordedframes;
};

// Stunts code
extern void __cdecl fatal_error(const char*, ...);
extern void __cdecl mmgr_alloc_a000(void);
extern void far* __cdecl mmgr_alloc_resbytes(const char* name, int32_t size);
extern void __cdecl init_kevinrandom(const char* seed);
extern int16_t __cdecl get_kevinrandom(void);
extern void __cdecl get_kevinrandom_seed(char* seed);
extern void __cdecl init_polyinfo(void);
extern int16_t __cdecl track_setup(void);
extern void __cdecl setup_aero_trackdata(void far* carresptr, uint16_t is_opponent);
extern void __cdecl load_opponent_data(void);
extern void far* __cdecl file_read_fatal(const char* filename, void far* dst);
extern void far* __cdecl file_load_resfile(const char* filename);
extern char far* __cdecl locate_shape_alt(const char far* data, const char* name);
extern void __cdecl restore_gamestate(uint16_t frame);
extern void __cdecl player_op(uint8_t);
extern void __cdecl opponent_op(void);
extern void __cdecl move_helicopters(void);
extern void __cdecl move_explode(void);

// Stunts data
#pragma aux g_language "*"
extern uint16_t g_language;
#pragma aux g_car_res_name "*"
extern char g_car_res_name[];
#pragma aux g_opponent_initials "*"
extern char g_opponent_initials;
#pragma aux g_rpfxrate "*"
extern uint16_t g_rpfxrate;
#pragma aux planptr "*"
extern void far* planptr;
#pragma aux wallptr "*"
extern void far* wallptr;
#pragma aux framespersec "*"
extern uint16_t framespersec;
#pragma aux run_game_random "*"
extern int16_t run_game_random;
#pragma aux gameconfig "*"
extern struct GAMEINFO gameconfig;
#pragma aux state "*"
extern struct GAMESTATE state;
#pragma aux cvxptr "*"
extern struct GAMESTATE far* cvxptr;
#pragma aux gabort "*"
extern uint8_t gabort;
#pragma aux trackrows "*"
extern int16_t trackrows[];
#pragma aux terrainrows "*"
extern int16_t terrainrows[];
#pragma aux trackpos "*"
extern int16_t trackpos[];
#pragma aux trackcenterpos "*"
extern int16_t trackcenterpos[];
#pragma aux terrainpos "*"
extern int16_t terrainpos[];
#pragma aux terraincenterpos "*"
extern int16_t terraincenterpos[];
#pragma aux trackpos2 "*"
extern int16_t trackpos2[];
#pragma aux trackcenterpos2 "*"
extern int16_t trackcenterpos2[];
#pragma aux td01_track_file_cpy "*"
extern int16_t far* td01_track_file_cpy;
#pragma aux td02_penalty_related "*"
extern int16_t far* td02_penalty_related;
#pragma aux trackdata3 "*"
extern int16_t far* trackdata3;
#pragma aux td04_aerotable_pl "*"
extern int16_t far* td04_aerotable_pl;
#pragma aux td05_aerotable_op "*"
extern int16_t far* td05_aerotable_op;
#pragma aux trackdata6 "*"
extern int8_t far* trackdata6;
#pragma aux trackdata7 "*"
extern int8_t far* trackdata7;
#pragma aux td08_direction_related "*"
extern int16_t far* td08_direction_related;
#pragma aux trackdata9 "*"
extern int16_t far* trackdata9;
#pragma aux td10_track_check_rel "*"
extern int16_t far* td10_track_check_rel;
#pragma aux td11_highscores "*"
extern int8_t far* td11_highscores;
#pragma aux trackdata12 "*"
extern int8_t far* trackdata12;
#pragma aux td13_rpl_header "*"
extern int8_t far* td13_rpl_header;
#pragma aux td14_elem_map_main "*"
extern uint8_t far* td14_elem_map_main;
#pragma aux td15_terr_map_main "*"
extern uint8_t far* td15_terr_map_main;
#pragma aux td16_rpl_buffer "*"
extern int8_t far* td16_rpl_buffer;
#pragma aux td17_trk_elem_ordered "*"
extern int8_t far* td17_trk_elem_ordered;
#pragma aux trackdata18 "*"
extern int8_t far* trackdata18;
#pragma aux trackdata19 "*"
extern uint8_t far* trackdata19;
#pragma aux td20_trk_file_appnd "*"
extern int8_t far* td20_trk_file_appnd;
#pragma aux td21_col_from_path "*"
extern int8_t far* td21_col_from_path;
#pragma aux td22_row_from_path "*"
extern int8_t far* td22_row_from_path;
#pragma aux trackdata23 "*"
extern uint8_t far* trackdata23;
