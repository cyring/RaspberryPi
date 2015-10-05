#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/sound.h>

class RootWindow
{
private:
	bool  quit;
	float x, y, U, V, step;
	int   W, H ;

public:
	void run()
	{
		quit = false;

		CL_DisplayWindowDescription window_desc;
		window_desc.set_size(CL_Size(1920, 1080), true);
		window_desc.set_title("Luna");
		CL_DisplayWindow window(window_desc);

		CL_Slot slot_quit = window.sig_window_close().connect(this, &RootWindow::on_window_close);

		CL_GraphicContext gc = window.get_gc();
		CL_InputDevice keyboard = window.get_ic().get_keyboard();

		CL_FontDescription font_desc;
		font_desc.set_typeface_name("Monospace");
		font_desc.set_height(16);
		CL_Font_System font(gc, font_desc);
		CL_FontMetrics fmetrics = font.get_font_metrics();
		int fntWidth = fmetrics.get_max_character_width();

		CL_SpriteDescription spr_desc;
		spr_desc.add_frame(CL_ImageProviderFactory::load("spaceship.png"));
		CL_Sprite node(gc, spr_desc);
		node.set_play_loop(true);
		node.set_play_pingpong(false);
		node.set_frame(0);

		CL_Image pict(gc,"stars.jpg");

		x = gc.get_width() / 2 ;
		y = gc.get_height() / 2 ;
		U = 0;
		W = 0;
		V = 0;
		H = 0;
		step = 0.2;

		while (!quit)
		{
			if(keyboard.get_keycode(CL_KEY_ESCAPE) == true)
				quit = true;
			if(keyboard.get_keycode(CL_KEY_LEFT) == true)
			    U -= step ;
			if(keyboard.get_keycode(CL_KEY_RIGHT) == true)
			    U += step ;
			if(keyboard.get_keycode(CL_KEY_UP) == true)
			    V -= step ;
			if(keyboard.get_keycode(CL_KEY_DOWN) == true)
			    V += step ;

			if(x < -node.get_width()) {
				x = gc.get_width();
			}
			else if(x > gc.get_width()) {
				x = -node.get_width();
			}
			else
				x += U ;

			if(y < step) {
				y = step;
				V = 0;
			}
			else if(y > gc.get_height()-node.get_height()) {
				y = gc.get_height()-node.get_height();
				V = 0;
			}
			else
				y += V;

			gc.clear();

			pict.draw(gc,W,H);
			pict.draw(gc,W,H-gc.get_height() );
			pict.draw(gc,W-gc.get_width(),H );
				H++ ;

			if( H > gc.get_height() ) H = 0;

			node.draw(gc, x, y);

			CL_String velocity = cl_format("Velocity U[%1] V[%2] --- Coordinate X[%3] Y[%4] --- Origin W[%5] H[%6]",U,V,x,y,W,H);
			font.draw_text(gc, gc.get_width()/2 - (velocity.length() * fntWidth)/2, 16, velocity);

			window.flip();
			CL_KeepAlive::process();
			CL_System::sleep(10);
		}
	}

	void on_window_close()
	{
		quit = true;
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		try
		{
			RootWindow root;
			root.run();
		}
		catch(CL_Exception &exception)
		{
			CL_ConsoleWindow console("Console", 80, 160);
			CL_Console::write_line("Error: " + exception.get_message_and_stack_trace());
			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

CL_ClanApplication app(&Program::main);
