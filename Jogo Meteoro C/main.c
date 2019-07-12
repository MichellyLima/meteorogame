///METEORO
///Created by Michelly Lima


#include <allegro.h>
#include <stdlib.h>

///VARIAVEIS GLOBAIS
volatile int exit_program;
void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int ticks;
void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

enum{ GAMEOVERSCREEN, TITLESCREEN, MAINMENU, GAMESCREEN };
int screen_state;
int vida_terra = 300;
int score = 0;
int score_maximo = 0;

void init()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 800, 600, 0, 0);
  set_window_title("Meteoro");

  exit_program = FALSE;
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);

  ticks = 0;
  LOCK_FUNCTION(tick_counter);
  LOCK_VARIABLE(ticks);
  install_int_ex(tick_counter, BPS_TO_TIMER(60));

  screen_state = TITLESCREEN;
}

 int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
    if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh){ return 1; }
    return 0;
  }
END_OF_FUNCTION(colidir)


int GameOver(int *met_x, float met_y, int nave_x, int nave_y){
    struct obj{
        int largura;
        int altura;
    };

    struct obj meteoros[10] = {50, 44, 45, 39, 28, 28, 17, 21, 18, 15, 50, 44, 45, 39, 28, 28, 17, 21, 18, 15};

    int i;

    for(i=0; i<10; i++){
        if(colidir(met_x[i], met_y, nave_x, nave_y, meteoros[i].largura, meteoros[i].altura, 40, 36)){
                return(1);
        }
    }

  return(0);
}
END_OF_FUNCTION(GameOver)


void mainmenu()
{
  int exit_screen = FALSE;

  ///BITMAPS
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
  BITMAP* met1 = load_bitmap("img/meteoro.bmp", NULL);
  BITMAP* met2 = load_bitmap("img/m2.bmp", NULL);
  BITMAP* met3 = load_bitmap("img/m3.bmp", NULL);
  BITMAP* met4 = load_bitmap("img/m4.bmp", NULL);
  BITMAP* met5 = load_bitmap("img/m5.bmp", NULL);
  BITMAP* controles = load_bitmap("img/menu.bmp", NULL);

  ///GAME LOOP
  while(!exit_program && !exit_screen)
  {
     while(ticks > 0 && !exit_program && !exit_screen)
     {
      //INPUT

      if(key[KEY_ENTER])
      {
        exit_screen = TRUE;
        screen_state = GAMESCREEN;
      }

      if(key[KEY_ESC])
      {
          exit_program = TRUE;
      }

      //UPDATE

      //DRAW
      draw_sprite(buffer, controles, 0, 0);
      draw_sprite(buffer, met1, 120, 220);
      draw_sprite(buffer, met2, 120, 290);
      draw_sprite(buffer, met3, 143, 360);
      draw_sprite(buffer, met4, 143, 425);
      draw_sprite(buffer, met5, 143, 490);
      textout_centre_ex(buffer, font, "ENTER PARA INICIAR", SCREEN_W/2, 576, makecol(255,255,255), -1);
      draw_sprite(screen, buffer, 0, 0);

     ticks--;
     }
  }

  ///FINALIZAÇAO
  destroy_bitmap(buffer);
  destroy_bitmap(controles);
  destroy_bitmap(met1);
  destroy_bitmap(met2);
  destroy_bitmap(met3);
  destroy_bitmap(met4);
  destroy_bitmap(met5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void titlescreen()
{
  int exit_screen = FALSE;


  ///BITMAPS
  BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
  BITMAP* telainicial = load_bitmap("img/titlescreen.bmp", NULL);
  BITMAP* meteoro = load_bitmap("img/meteoro.bmp", NULL);

  ///VARIAVEIS
  float teta = 0.0;

  ///GAME LOOP
  while(!exit_program && !exit_screen)
  {
     while(ticks > 0 && !exit_program && !exit_screen)
     {
      //INPUT
      if(key[KEY_ENTER])
      {
        exit_screen = TRUE;
        screen_state = MAINMENU;
      }

      if(key[KEY_ESC])
      {
          exit_program = TRUE;
      }

      //UPDATE
      teta += 0.5;

      //DRAW
      draw_sprite(buffer, telainicial, 0, 0);
      rotate_sprite(buffer, meteoro, 559, 61, ftofix(teta));
      rotate_sprite(buffer, meteoro, 57, 315, ftofix(teta));
      rotate_sprite(buffer, meteoro, 695, 406, ftofix(teta));
      textprintf_ex(buffer, font, 590, 569, makecol(255,255,255), -1, "Created by Michelly Lima");
      textout_centre_ex(buffer, font, "APERTE ENTER PARA CONTINUAR", SCREEN_W/2, 412, makecol(255,255,255), -1);
      draw_sprite(screen, buffer, 0, 0);

     ticks--;
     }
  }

  ///FINALIZAÇAO
  destroy_bitmap(buffer);
  destroy_bitmap(meteoro);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void gamescreen(){

    int exit_screen = FALSE;
    ///SONS
    SAMPLE* musicGameOver = load_wav("msc/gameover.wav");
    SAMPLE* efeito = load_sample("msc/tiro.wav");
    SAMPLE* explosao = load_sample("msc/efeitoexplosao.wav");

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo = load_bitmap("img/fundo.bmp", NULL);
    BITMAP* invad = load_bitmap("img/imagem.bmp", NULL);
    BITMAP* ship = load_bitmap("img/nave.bmp", NULL);
    BITMAP* m1 = load_bitmap("img/meteoro.bmp", NULL);
    BITMAP* m2 = load_bitmap("img/m2.bmp", NULL);
    BITMAP* m3 = load_bitmap("img/m3.bmp", NULL);
    BITMAP* m4 = load_bitmap("img/m4.bmp", NULL);
    BITMAP* m5 = load_bitmap("img/m5.bmp", NULL);
    BITMAP* explosion = load_bitmap("img/explosion.bmp", NULL);

    ///VARIAVEIS LOCAIS
    int estado_anterior_SPACE;

    int cont_colisao[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int atirar = 0;

    int x_explosion[10];

    struct obj{
      int x;
      int y;
      int wx;
      int wy;
      int w;
      int h;
    };

    struct obj tiro = {-1, -1, 420, 504, 3, 8};

    int x = SCREEN_W/2, y = 500;
    int x_meteoro[10];
    float h_meteoro = 0, velocidade = 0.3; //velocidade do h_meteoro
    int cy = 0;
    int i, j;
    int V = 0;

    for(i = 0; i < 10; i++){
       x_meteoro[i] = rand()%SCREEN_W;
    }

    for(i=0; i < 10; i++){
        x_explosion[i] = x_meteoro[i];
    }

    /// GAME LOOP
    while(!exit_program && !exit_screen)
  {
     while(ticks > 0 && !exit_program && !exit_screen)
     {
      //INPUT
      estado_anterior_SPACE = key[KEY_SPACE];
	    poll_keyboard();

	    tiro.y -= 45;

      if(key[KEY_ESC])
       fecha_programa();

      if(estado_anterior_SPACE == 0 && key[KEY_SPACE] != 0 && atirar == 0)
          play_sample(efeito, 50, 128, 1000, FALSE);

      //UPDATE

      h_meteoro += velocidade;

      if( cy > 600){ cy = 0; }
      cy += velocidade;

     ///CONTROLE DA NAVE
     if(key[KEY_A] || key[KEY_LEFT]){ x -= 6; }
     else if(key[KEY_D] || key[KEY_RIGHT]){ x += 6; }

     if(key[KEY_SPACE]){ atirar = 1; }
     if(!atirar){ tiro.x = x+20; tiro.y = y+2;}
     if(tiro.y < 15 ){ atirar = 0;}

     if( x>SCREEN_W){ x = 0; }
     if(x<0){ x = SCREEN_W; }

      //DRAW

      draw_sprite(buffer, fundo, 0, cy);
      draw_sprite(buffer, fundo, 0, cy-600);

     draw_sprite_ex(buffer, ship, x, y, DRAW_SPRITE_NORMAL, DRAW_SPRITE_NO_FLIP);

                  //Game Over
     if(GameOver(x_meteoro, h_meteoro, x, y) == 1 || vida_terra < 0){
            play_sample(musicGameOver, 50, 128, 1000, FALSE);
            exit_screen = TRUE;
            screen_state = GAMEOVERSCREEN;
     }

        draw_sprite(buffer, m1, x_meteoro[0], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[0], h_meteoro, tiro.w, tiro.h, 62, 59)){
                cont_colisao[0]++;
                atirar = 0;
                if(cont_colisao[0] == 4){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[0] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[0], h_meteoro, 58, 50);
                    score += 40;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[0] != SCREEN_W+100){ vida_terra -= 40; }


        draw_sprite(buffer, m2, x_meteoro[1], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[1], h_meteoro, tiro.w, tiro.h, 45, 39)){
                cont_colisao[1]++;
                atirar = 0;
                if(cont_colisao[1] == 3){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[1] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[1], h_meteoro, 58, 50);
                    score += 30;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[1] != SCREEN_W+100){ vida_terra -= 30; }

        draw_sprite(buffer, m3, x_meteoro[2], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[2], h_meteoro, tiro.w, tiro.h, 30, 28)){
                cont_colisao[2]++;
                atirar = 0;
                if(cont_colisao[2] == 2){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[2] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[2], h_meteoro, 58, 50);
                    score += 20;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[2] != SCREEN_W+100){ vida_terra -= 20; }

        draw_sprite(buffer, m4, x_meteoro[3], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[3], h_meteoro, tiro.w, tiro.h, 35, 30)){
                cont_colisao[3]++;
                atirar = 0;
                if(cont_colisao[3] == 1){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[3] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[3], h_meteoro, 58, 50);
                    score += 10;
                }
            }

        if(h_meteoro > SCREEN_H && x_meteoro[3] != SCREEN_W+100){ vida_terra -= 10; }

        draw_sprite(buffer, m5, x_meteoro[4], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[4], h_meteoro, tiro.w, tiro.h, 48, 25)){
                cont_colisao[4]++;
                atirar = 0;
                if(cont_colisao[4] == 1){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[4] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[4], h_meteoro, 58, 50);
                    score += 10;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[4] != SCREEN_W+100){ vida_terra -= 10; }

        draw_sprite(buffer, m1, x_meteoro[5], h_meteoro);
             if(colidir(tiro.x, tiro.y, x_meteoro[5], h_meteoro, tiro.w, tiro.h, 62, 59)){
                cont_colisao[5]++;
                atirar = 0;
                if(cont_colisao[5] == 4){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[5] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[5], h_meteoro, 58, 50);
                    score += 40;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[5] != SCREEN_W+100){ vida_terra -= 40; }

        draw_sprite(buffer, m2, x_meteoro[6], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[6], h_meteoro, tiro.w, tiro.h, 45, 39)){
                cont_colisao[6]++;
                atirar = 0;
                if(cont_colisao[6] == 3){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[6] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[6], h_meteoro, 58, 50);
                    score += 30;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[6] != SCREEN_W+100){ vida_terra -= 30; }

        draw_sprite(buffer, m3, x_meteoro[7], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[7], h_meteoro, tiro.w, tiro.h, 30, 28)){
                cont_colisao[7]++;
                atirar = 0;
                if(cont_colisao[7] == 2){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[7] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[7], h_meteoro, 58, 50);
                    score += 20;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[7] != SCREEN_W+100){ vida_terra -= 20; }

        draw_sprite(buffer, m4, x_meteoro[8], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[8], h_meteoro, tiro.w, tiro.h, 35, 30)){
                cont_colisao[8]++;
                atirar = 0;
                if(cont_colisao[8] == 1){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[8] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[8], h_meteoro, 58, 50);
                    score += 10;
                }
            }

        if(h_meteoro > SCREEN_H && x_meteoro[8] != SCREEN_W+100){ vida_terra -= 10; }

        draw_sprite(buffer, m5, x_meteoro[9], h_meteoro);
            if(colidir(tiro.x, tiro.y, x_meteoro[9], h_meteoro, tiro.w, tiro.h, 48, 25)){
                cont_colisao[9]++;
                atirar = 0;
                if(cont_colisao[9] == 1){
                    play_sample(explosao, 50, 128, 1000, FALSE);
                    x_meteoro[9] = SCREEN_W + 100;
                    V++;
                    masked_blit(explosion, buffer, -1, -1, x_explosion[9], h_meteoro, 58, 50);
                    score += 10;
                }
            }

            if(h_meteoro > SCREEN_H && x_meteoro[9] != SCREEN_W+100){ vida_terra -= 10; }

     if(atirar){ masked_blit(invad, buffer, tiro.wx, tiro.wy, tiro.x, tiro.y, tiro.w, tiro.h);}

     textprintf_ex(buffer, font, 10, 550, makecol(255,255,255), -1, "Score: %d", score);
     textprintf_ex(buffer, font, 10, 570, makecol(255,255,255), -1, "Score Maximo: %d", score_maximo);
     textprintf_ex(buffer, font, 620, 550, makecol(255,0,0), -1, "Vida da Terra: %d", vida_terra);

      if(h_meteoro > SCREEN_H || V == 10){

        for(i = 0; i < 10; i++){
            x_meteoro[i] = rand()%SCREEN_W;
        }

        for(i=0; i < 10; i++){
            x_explosion[i] = x_meteoro[i];
        }

        h_meteoro = 0;
        velocidade += 0.2;

        for(i=0; i< 10; i++){ cont_colisao[i] = 0; }

        V = 0;

     }

     draw_sprite(screen, buffer, 0, 0);

     clear(buffer);

  }

     ticks--;
     }

  ///FINALIZACAO
  destroy_sample(efeito);
  destroy_sample(explosao);
  destroy_bitmap(fundo);
  destroy_bitmap(ship);
  destroy_bitmap(m1);
  destroy_bitmap(m2);
  destroy_bitmap(m3);
  destroy_bitmap(m4);
  destroy_bitmap(m5);
  destroy_bitmap(buffer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void acabou(){
    int exit_screen = 0;
    int estado_anterior_ENTER;


    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* fundo = load_bitmap("img/fundo.bmp", NULL);

    ///GAME LOOP
     while(!exit_program && exit_screen == 0)
  {
     while(ticks > 0 && !exit_program && !exit_screen)
     {
      //INPUT
      estado_anterior_ENTER = key[KEY_ENTER];
	    poll_keyboard();


      if(estado_anterior_ENTER == 0 && key[KEY_ENTER] != 0){
        exit_screen = 1;
        score = 0;
        screen_state = GAMESCREEN;

      }

      if(key[KEY_ESC]){
        exit_program = TRUE;
      }

      //UPDATE
      if(score >= score_maximo){void init()
{
  allegro_init();
  install_timer();
  install_keyboard();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  set_window_title("Meteoro");

  exit_program = FALSE;
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);

  ticks = 0;
  LOCK_FUNCTION(tick_counter);
  LOCK_VARIABLE(ticks);
  install_int_ex(tick_counter, BPS_TO_TIMER(60));

  screen_state = TITLESCREEN;
}

            score_maximo = score;

      }

        vida_terra = 300;

      //DRAW
      draw_sprite(buffer, fundo, 0, 0);
      textout_centre_ex(buffer, font, "VOCE PERDEU! ENTER PARA TENTAR DE NOVO", SCREEN_W/2, SCREEN_H/2, makecol(255,255,255), -1);
      textprintf_ex(buffer, font, 343, 364, makecol(255,255,255), -1, "Score: %d", score);
      textprintf_ex(buffer, font, 343, 394, makecol(255,255,255), -1, "Score Maximo: %d", score_maximo);
      draw_sprite(screen, buffer, 0, 0);

     ticks--;
     }
  }

  ///FINALIZACAO
  destroy_bitmap(buffer);
  destroy_bitmap(fundo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
  init();

  ///SONS
  MIDI* musica = load_midi("msc/space.mid");

  while(!exit_program)
  {
     play_midi(musica, TRUE);
     if(screen_state == MAINMENU)
       mainmenu();

     else if(screen_state == TITLESCREEN)
       titlescreen();

     else if(screen_state == GAMESCREEN)
        gamescreen();

     else if(screen_state == GAMEOVERSCREEN)
         acabou();
  }
  destroy_midi(musica);
  return 0;
}
END_OF_MAIN();

