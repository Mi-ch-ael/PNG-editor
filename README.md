# PNG-editor

Second term coursework: PNG image editor written in C++. GUI created with Qt5, image manipulation done with [libpng](http://www.libpng.org/pub/png/libpng.html).

## Installation

Granted that you have both Qt and libpng (version 1.2.x >= 1.2.50) installed, run `qmake` and then `make` in `src` directory or simply build the project via Qt Creator.

## Usage

Press folder icon (alternatively, File - Open or `Ctrl+O`) and choose a `.png` file in a dialog that opens to load an image. Paletted images are not supported.

To save an image, press "Save" icon, File - Save or `Ctrl+S`, then choose image name and image location in a dialog.

Press dice icon, File - Property or `Ctrl+I` to see image properties. The app converts any loaded picture to RGBA format with 8-bit color depth without interlacing, so the "after conversion" part must always be the same.

The app supports four kinds of image transformations: dividing an image with horizontal and vertical lines, drawing rectangles, rotating picture fragments and drawing frames.

Choose line color for "divide", "rectangle" and "frame" tools from "color" list. Choose rectangle fill color from "color fill" list. Choose line width for "divide" and "rectangle" tools from "line" list.

To use "divide" tool, enter number of horizontal parts (created by vertical lines) and number of vertical parts (created by horizontal lines) in "H" and "V" text fields respectively. Entering values more than number of pixels in the specified direction has no effect, as well as entering 0. Entering 1 does nothing too, because the image is already in one piece. Press button with four squares or `Ctrl+D` to divide an image.

To use "rotate" tool, click on the image twice to specify the rectangular rotation region, then choose an angle from a list next to rotation button. Click the rotation button or `Ctrl+R` to rotate specified region around its center.

To draw a rectangle, click on the image twice to specify opposite corners of the rectangle, then click "rectangle" button or `Ctrl+T`.

To draw a frame, choose a pattern from the "pattern" list, choose frame width from the "width" list and click "frame" button or `Ctrl+F`.

Click About - About for a quick help.
