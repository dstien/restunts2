#include "util.h"
#include <externs.h>
#include <restunts.h>

void init_track_tables(void)
{
    for (int i = 0; i < 30; i++) {
        trackrows[i] = 30 * (29 - i);
        terrainrows[i] = 30 * i;
        trackpos[i] = (29 - i) << 10;
        trackcenterpos[i] = ((29 - i) << 10) + 0x200;
        terrainpos[i] = i << 10;
        terraincenterpos[i] = (i << 10) + 0x200;
    }

    for (int i = 0; i < 30; i++) {
        trackpos2[i] = i << 10;
        trackcenterpos2[i] = (i << 10) + 0x200;
    }
}

void init_track_pointers(void)
{
    uint8_t far* trkptr = mmgr_alloc_resbytes("trakdata", 0x6BF3);

    td01_track_file_cpy = (void far*)trkptr;

    trkptr += 0x70a;
    td02_penalty_related = (void far*)trkptr;

    trkptr += 0x70a;
    trackdata3 = (void far*)trkptr;

    trkptr += 0x70a;
    td04_aerotable_pl = (void far*)trkptr;

    trkptr += 0x80;
    td05_aerotable_op = (void far*)trkptr;

    trkptr += 0x80;
    trackdata6 = (void far*)trkptr;

    trkptr += 0x80;
    trackdata7 = (void far*)trkptr;

    trkptr += 0x80;
    td08_direction_related = (void far*)trkptr;

    trkptr += 0x60;
    trackdata9 = (void far*)trkptr;

    trkptr += 0x180;
    td10_track_check_rel = (void far*)trkptr;

    trkptr += 0x120;
    td11_highscores = (void far*)trkptr;

    trkptr += 0x16c;
    trackdata12 = (void far*)trkptr;

    trkptr += 0x0f0;
    td13_rpl_header = (void far*)trkptr;

    trkptr += 0x1a;
    td14_elem_map_main = (void far*)trkptr;

    trkptr += 0x385;
    td15_terr_map_main = (void far*)trkptr;

    trkptr += 0x385;
    td16_rpl_buffer = (void far*)trkptr;

    trkptr += 0x2ee0;
    td17_trk_elem_ordered = (void far*)trkptr;

    trkptr += 0x385;
    trackdata18 = (void far*)trkptr;

    trkptr += 0x385;
    trackdata19 = (void far*)trkptr;

    trkptr += 0x385;
    td20_trk_file_appnd = (void far*)trkptr;

    trkptr += 0x7ac;
    td21_col_from_path = (void far*)trkptr;

    trkptr += 0x385;
    td22_row_from_path = (void far*)trkptr;

    trkptr += 0x385;
    trackdata23 = (void far*)trkptr;

    trkptr += 0x30;
}

void init_stunts(const char* replay_filename)
{
    printf("%-30s", "Initialising game...");
    g_language = 'e';
    framespersec = 20;

    init_kevinrandom("kevin");
    run_game_random = get_kevinrandom() << 3;

    mmgr_alloc_a000();
    init_polyinfo();
    init_track_tables();
    init_track_pointers();
    printf("OK\n");

    printf("%-30s", "Loading replay...");
    file_read_fatal(replay_filename, td13_rpl_header);
    gameconfig = *(struct GAMEINFO far*)td13_rpl_header;
    printf("OK (%s)\n", gameconfig.game_trackname);

    printf("%-30s", "Setting up track...");
    track_setup();
    printf("OK\n");

    printf("%-30s", "Allocating CVX...");
    cvxptr = mmgr_alloc_resbytes("cvx", 0x5780);
    printf("OK\n");

    printf("%-30s", "Loading game assets...");
    void far* gameres = file_load_resfile("game");
    planptr = locate_shape_alt(gameres, "plan");
    wallptr = locate_shape_alt(gameres, "wall");
    printf("OK\n");

    printf("%-30s", "Loading player...");
    memcpy(&g_car_res_name[3], gameconfig.game_playercarid, 4);
    setup_aero_trackdata(file_load_resfile(g_car_res_name), 0);
    printf("OK (%.4s)\n", gameconfig.game_playercarid);

    if (gameconfig.game_opponenttype) {
        printf("%-30s", "Loading opponent...");
        memcpy(&g_car_res_name[3], gameconfig.game_opponentcarid, 4);
        setup_aero_trackdata(file_load_resfile(g_car_res_name), 1);
        load_opponent_data();
        printf("OK (%.4s, %.2s)\n", gameconfig.game_opponentcarid,
               &g_opponent_initials);
    }

    printf("%-30s", "Initialising state...");
    restore_gamestate(0);
    restore_gamestate(gameconfig.game_recordedframes);
    state.game_inputmode = 1;
    printf("OK\n");
}

int dump(const char* output_filename)
{
    printf("Creating file '%s'...", output_filename);
    FILE* fout = fopen(output_filename, "w");
    if (!fout) {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");

    fwrite(&gameconfig.game_recordedframes, sizeof(uint16_t), 1, fout);
    printf("Processing %d frames...     ", gameconfig.game_recordedframes);

    while (gameconfig.game_recordedframes > state.game_frame) {
        // Allow extra frames to render when crashing. Doesn't affect
        // simulation, but as these stats are stored in the game state, we mimic
        // the main game loop so we later can compare if our output is identical
        // to the original code when running tests.
        if (state.game_3F6autoLoadEvalFlag != 0 &&
            state.game_frame_in_sec < state.game_frames_per_sec) {
            state.game_frame_in_sec++;
            if (state.game_frame_in_sec == state.game_frames_per_sec &&
                state.playerstate.car_crashBmpFlag == 1 &&
                state.playerstate.car_speed2 != 0 && gabort == 0) {
                state.game_frames_per_sec++;
            }
        }

        // Update random seed in state every 600th frame.
        if (state.game_frame % g_rpfxrate == 0) {
            get_kevinrandom_seed((char*)state.kevinseed);
        }

        uint8_t input = td16_rpl_buffer[state.game_frame];
        state.game_frame++;

        // Update the player simulation for the current frame.
        player_op(input);

        // Not needed for player simulation, but opponent, cameras and explosions are
        // tracked in the game state, so we include it for completeness.
        if (gameconfig.game_opponenttype) {
            opponent_op();
        }
        move_helicopters();
        if (state.field_42A != 0) {
            move_explode();
        }

        fwrite(&state, sizeof(struct GAMESTATE), 1, fout);
    }

    fclose(fout);
    printf("OK\n");

    return 0;
}

void usage(const char* prog)
{
    printf("Usage: %s [-o <outfile>] <replname>\n", prog);
    printf("  -h, -?        Print usage\n");
    printf("  -v            Print version\n");
    printf("  -o <outfile>  Use specified output file\n");
    printf("  <replname>    Required Stunts replay file\n\n");
}

int repldumpmain(int argc, char* argv[])
{
    const char* in_arg = 0;
    const char* out_arg = 0;

    for (int i = 1; i < argc; i++) {
        const char* a = argv[i];
        if (a[0] == '-') {
            if (strcmp(a, "-?") == 0 || stricmp(a, "-h") == 0) {
                usage(argv[0]);
                return 0;
            }
            else if (stricmp(a, "-v") == 0) {
                printf("restunts repldump for " PLATFORM_STR "\n");
                printf("Build date: " DATE "\n\n");
                return 0;
            }
            else if (stricmp(a, "-o") == 0) {
                if (argc <= ++i) {
                    usage(argv[0]);
                    return 2;
                }
                out_arg = argv[i];
            }
            else {
                usage(argv[0]);
                return 2;
            }
        }
        else {
            if (in_arg) {
                usage(argv[0]);
                return 2;
            }
            in_arg = a;
        }
    }
    if (!in_arg) {
        usage(argv[0]);
        return 2;
    }
    char out_filename[128];
    if (out_arg) {
        strcpy(out_filename, out_arg);
    }
    else {
        int ext_pos = 0;
        char* in_ext = strrchr(in_arg, '.');
        if (in_ext) {
            ext_pos = in_ext - in_arg;
        }
        else {
            ext_pos = strlen(in_arg);
        }
        memcpy(&out_filename, in_arg, ext_pos);
        strcpy(&out_filename[ext_pos], ".bin");
    }

    init_stunts(in_arg);
    return dump(out_filename);
}
