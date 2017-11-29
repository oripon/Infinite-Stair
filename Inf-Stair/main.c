#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <time.h>
#include <stdlib.h>
#include "../include/MakeQueue.h"

#define block_dist_y 22
#define block_dist_x 40

#define MAX_QUEUE_SIZE 30

int queue[MAX_QUEUE_SIZE];

//Objs
GtkWidget *window;
GtkWidget *fixed;
GtkWidget *UpButton;
GtkWidget *TurnButton;
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
int On_Anim = 0;
int block_pos = 0;

//###############################
//Threads
//###############################

//Title Anim
static gpointer Title_Anim_thread()
{
	int ypos = -100;
	for(int i = 0; i < 100; i++)
	{
		ypos += 2;
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		usleep(10000);
	}
	for(int i = 0; i < 5; i++)
	{
		ypos -= 2;
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		usleep(10000);
	}
	for(int i = 0; i < 5; i++)
	{
		ypos += 2;
		gtk_fixed_move(GTK_FIXED(fixed),Title,45,ypos);
		usleep(10000);
	}

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
			On_Anim = 0;
			usleep(300000);
			On_Anim = 1;
			usleep(300000);
			On_Anim = 2;
			usleep(300000);
			On_Anim = 3;
			usleep(300000);
		}
	}
	return NULL;
}
//Charactor Move anim
void Set_blocks()
{
	for(int i = 0; i<30; i++)
	{
		gtk_fixed_move(GTK_FIXED(fixed),block[i],155+(queue[i]-4)*block_dist_x,625-i*block_dist_y);
	}
}
gpointer Charactor_Anim_Move_thread()
{
	while(1)
	{
		for(int i = 0; i<=11; i++)
		{
			gtk_widget_hide(Charactor[i]);
		}
		gtk_widget_show(Charactor[On_Anim]);
		for(int i = 0; i<30; i++)
		{
			gtk_fixed_move(GTK_FIXED(fixed),block[i],155+(queue[i] - 4 + block_pos)*block_dist_x,625-i*block_dist_y);
		}
		//Set_blocks();
		usleep(50000);
	}
	g_thread_exit(Move_thread);
	return NULL;
}
gpointer Charactor_On_Anim_thread()
{
	while(1)
	{
		if(isMove == 1)
		{
			isMove = 0;

			On_Anim = 4 + isDirRight * 4;
			usleep(50000);
			On_Anim = 5 + isDirRight * 4;
			usleep(50000);
			On_Anim = 6 + isDirRight * 4;
			usleep(50000);
			On_Anim = 7 + isDirRight * 4;
			usleep(50000);
		}
	}
	g_thread_exit(On_Anim_thread);
	return NULL;
}

//###############################
//signal funcs
//###############################
gboolean UpButton_Clicked(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	if(event ->keyval == GDK_Up)
	{
		isFirstMove = 1;
		isMove= 1;
		block_pos = block_pos + 1 - (isDirRight*2);

		dequeue(queue);
		insertRightValue(queue);
	}
	if(event ->keyval == GDK_space)
	{
		isFirstMove = 1;
		isMove= 1;
		if(isDirRight == 1)
		{
			isDirRight = 0;
		}
		else
		{
			isDirRight = 1;
		}
		block_pos = block_pos + 1 - (isDirRight*2);

		dequeue(queue);
		insertRightValue(queue);
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

	//Title
	Title = gtk_image_new_from_file("imgs/Title.png");
	gtk_widget_show(Title);

	//UpButton
	UpButton = gtk_button_new_with_label("Up");
	gtk_widget_set_size_request(UpButton,50,50);
	gtk_widget_show(UpButton);

	//UpButton
	TurnButton = gtk_button_new_with_label("Turn");
	gtk_widget_set_size_request(TurnButton,50,50);
	gtk_widget_show(TurnButton);

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
	gtk_fixed_put(GTK_FIXED(fixed),Title,45,-100);

	//Buttons
	gtk_fixed_put(GTK_FIXED(fixed),UpButton,270,500);
	gtk_fixed_put(GTK_FIXED(fixed),TurnButton,50,500);

	//4 -> 155,625-6*22
	//y = 22
	//x = 40
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
	Move_thread = g_thread_new(NULL, Charactor_Anim_Move_thread,NULL);
	On_Anim_thread = g_thread_new(NULL, Charactor_On_Anim_thread,NULL);
	Init_blocks(queue);
	gtk_main();

	g_thread_join(Title_thread);
	g_thread_join(Idle_thread);
	g_thread_join(Move_thread);
	g_thread_join(On_Anim_thread);

	return 0;
}
