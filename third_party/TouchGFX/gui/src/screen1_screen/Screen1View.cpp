#include <gui/screen1_screen/Screen1View.hpp>
#include "BitmapDatabase.hpp"

Screen1View::Screen1View()
{
    progress = 0;
}

void Screen1View::setupScreen()
{
    update_gfx();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::update_gfx()
{
    if (progress < 100)
    {
        button1.setBitmaps(Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
        button1.setTouchable(true);
    }

    boxProgress1.setValue(progress);
    // Force redraw
    button1.invalidate();
    boxProgress1.invalidate();
}

void Screen1View::IncreaseBar()
{
    progress = (progress++ > 100) ? 100 : progress;
    update_gfx();
}