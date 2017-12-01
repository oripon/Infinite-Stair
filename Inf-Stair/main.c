#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>

#include <time.h>
#include <stdlib.h>
#include "../include/MakeQueue.h"

#define block_dist_y 22
#define block_dist_x 40

#define MAX_QUEUE_SIZE 30

int queue[MAX_QUEUE_SIZE];
int queue_prev[MAX_QUEUE_SIZE];
//Objs
GtkWidget *window;
GtkWidget *fixed;
//Images
GtkWidget *BG;
GtkWidget *Title;
GtkWidget *Charactor[12];
GtkWidget *block[30];

GThread *Title_thread;
GThread *Idle_thread;
GThread *Move_thread;
GThread *On_Anim_thread;
//Values
int isDirRight = 0;
int isFirstMove = 0;
int isMove = 0;
int isBlockMove = 0;
int Prev_Char_Anim = 0;
int block_pos = 0;
int block_five_pos = 0;
int isDie = 0;

void Change_Char_Anim(int n)
{
	for(int i = 0; i<=11; i++)
	{
		gdk_threads_enter();
		gtk_widget_hide(Charactor[i]);
		gdk_threads_leave();
	}
	gdk_threads_enter();
	gtk_widget_show(Charactor[n]);
	gdk_threads_leave();
}
void Change_Block_Anim(int x, int y)
{
	for(int i = 0; i<30; i++)
	{
		gdk_threads_enter();
		gtk_fixed_move(GTK_FIXED(fixed),block[i],155+(queue[i] - 4 + block_pos)*block_dist_x + x,625-i*block_dist_y + y);
		gdk_threads_leave();
	}
}
//###############################
//Threads
//###############################

//Title Anim
static gpointer Title_Anim_thread()
{
	int ypos = -100;
	isDie = 1;
	for(int i = 0; i < 100; i++)
	{
		ypos += 2;
		gdk_threads_enter();
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		gdk_threads_leave();
		usleep(10000);
	}
	for(int i = 0; i < 5; i++)
	{
		ypos -= 2;
		gdk_threads_enter();
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		gdk_threads_leave();
		usleep(10000);
	}
	for(int i = 0; i < 5; i++)
	{
		ypos += 2;
		gdk_threads_enter();
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		gdk_threads_leave();
		usleep(10000);
	}
	isDie = 0;

	g_thread_exit(Title_thread);
	return NULL;
}
//Charactor Idle Anim
static gpointer Charactor_Anim_Idle_thread()
{
	while(1)
	{
		for(int j = 0; j<4; j++)
		{
			if(isFirstMove==1)
			{
				g_thread_exit(Idle_thread);
			}
			Change_Char_Anim(0);
			//On_Anim = 0;
			usleep(300000);
			Change_Char_Anim(1);
			//On_Anim = 1;
			usleep(300000);
			Change_Char_Anim(2);
			//On_Anim = 2;
			usleep(300000);
			Change_Char_Anim(3);
			//On_Anim = 3;
			usleep(300000);
		}
	}
	return NULL;
}
//Block Move anim
gpointer Block_Move_thread()
{
	while(1)
	{
		if(isBlockMove == 1)
		{
			isBlockMove = 0;
			if(isDirRight == 0)
			{
				for(int i = -40; i<=0; i+=5)
				{
					Change_Block_Anim(i, i/2);
					usleep(5000);
				}
			}
			else
			{
				for(int i = 40; i>=0; i-=5)
				{
					Change_Block_Anim(i, -i/2);
					usleep(5000);
				}
			}
		}
	}
	g_thread_exit(Move_thread);
	return NULL;
}
gpointer Charactor_On_Anim_thread()
{
	while(1)
	{
		usleep(100);
		if(isMove == 1)
		{
			isMove = 0;

			Change_Char_Anim(4 + isDirRight * 4);
			usleep(50000);
			Change_Char_Anim(5 + isDirRight * 4);
			usleep(50000);
			Change_Char_Anim(6 + isDirRight * 4);
			usleep(50000);
			Change_Char_Anim(7 + isDirRight * 4);
			usleep(50000);

			//Die Condition
			if(queue[6] - 4 + block_pos != 0)
			{
				isDie = 1;
				sleep(1);
				for(int i = 0; i<100; i++)
				{
					for(int j = 0; j<=11; j++)
					{
						gdk_threads_enter();
						gtk_fixed_move(GTK_FIXED(fixed),Charactor[j],140,350+i*10);
						gdk_threads_leave();
					}
					usleep(7000);
				}
			}
		}
	}
	g_thread_exit(On_Anim_thread);
	return NULL;
}

//###############################
//signal funcs
//###############################
void Block_Ctrl()
{	
	block_pos = block_pos + 1 - (isDirRight*2);
	for(int i = 0; i<MAX_QUEUE_SIZE; i++)
	{
		queue_prev[i] = queue[i];
	}
	dequeue(queue);
	insertRightValue(queue);
}
gboolean UpButton_Clicked(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	if(isDie == 0)
	{
		if(event ->keyval == GDK_Up)
		{
			isFirstMove = 1;
			isMove= 1;
			isBlockMove = 1;
			Block_Ctrl();
		}
		if(event ->keyval == GDK_space)
		{
			gtk_widget_hide(Title);
			isFirstMove = 1;
			isMove= 1;
			isBlockMove = 1;
			if(isDirRight == 1)
			{
				isDirRight = 0;
			}
			else
			{
				isDirRight = 1;
			}
			Block_Ctrl();
		}
	}
	return 1;
}
void destroy()
{
	gtk_main_quit();
	exit(0);
}



//###############################
int main(int argc, char *argv[])
{
	gdk_threads_init();
	gdk_threads_enter();
	gtk_init(&argc,&argv);

	//#################
	//Init
	//#################

	//window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(destroy),NULL);
	gtk_window_resize(GTK_WINDOW(window),360,640);
	gtk_widget_show(window);

	//BG
	BG = gtk_image_new_from_file("imgs/BG.png");
	gtk_widget_show(BG);




	//block
	for(int i = 0; i<30; i++)
	{
		block[i] = gtk_image_new_from_file("imgs/block.png");
		gtk_widget_show(block[i]);
	}

	//Charactor
	Charactor[0] = gtk_image_new_from_file("imgs/char/idle1.png");//FirstIdle
	Charactor[1] = gtk_image_new_from_file("imgs/char/idle2.png");
	Charactor[2] = gtk_image_new_from_file("imgs/char/idle3.png");
	Charactor[3] = gtk_image_new_from_file("imgs/char/idle4.png");

	Charactor[4] = gtk_image_new_from_file("imgs/char/move1.png");//Move_Left
	Charactor[5] = gtk_image_new_from_file("imgs/char/move2.png");
	Charactor[6] = gtk_image_new_from_file("imgs/char/move3.png");

	Charactor[7] = gtk_image_new_from_file("imgs/char/idle_after_move.png");//IdleAfterMove_Left

	Charactor[8] = gtk_image_new_from_file("imgs/char/move4.png");//Move_Right
	Charactor[9] = gtk_image_new_from_file("imgs/char/move5.png");
	Charactor[10] = gtk_image_new_from_file("imgs/char/move6.png");

	Charactor[11] = gtk_image_new_from_file("imgs/char/idle_after_move_right.png");//IdleAfterMove_Right

	//################
	//Position
	//################
	fixed = gtk_fixed_new();
	gtk_widget_show(fixed);
	gtk_widget_set_size_request(fixed,0,0);
	gtk_container_add(GTK_CONTAINER(window),fixed);

	gtk_fixed_put(GTK_FIXED(fixed),BG,0,0);
	//gtk_fixed_put(GTK_FIXED(fixed),Title,45,-100);

	//Blocks
	for(int i = 0; i<30; i++)
	{
		gtk_fixed_put(GTK_FIXED(fixed),block[i],155+i*block_dist_x,625-i*block_dist_y);
	}

	//Charactor
	for(int i = 0; i<=11; i++)
	{
		gtk_fixed_put(GTK_FIXED(fixed),Charactor[i],140,350);
	}


	//Signals
	gtk_signal_connect(GTK_OBJECT(window),"key-press-event",G_CALLBACK(UpButton_Clicked),NULL);

	//####################
	//Thread
	//####################

	Title_thread = g_thread_new(NULL, Title_Anim_thread,NULL);
	Idle_thread = g_thread_new(NULL, Charactor_Anim_Idle_thread,NULL);
	Move_thread = g_thread_new(NULL, Block_Move_thread,NULL);
	On_Anim_thread = g_thread_new(NULL, Charactor_On_Anim_thread,NULL);

	Init_blocks(queue);
	for(int i = 0; i<30; i++)
	{
		gtk_fixed_move(GTK_FIXED(fixed),block[i],155+(queue[i] - 4 + block_pos)*block_dist_x,625-i*block_dist_y);
	}
	//Title
	Title = gtk_image_new_from_file("imgs/Title.png");
	gtk_widget_show(Title);
	gtk_fixed_put(GTK_FIXED(fixed),Title,45,-100);

	gtk_main();
	gdk_threads_leave();

	g_thread_join(Title_thread);
	g_thread_join(Idle_thread);
	g_thread_join(Move_thread);
	g_thread_join(On_Anim_thread);

	return 0;
}
