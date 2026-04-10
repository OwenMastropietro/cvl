// Example: Hough Circle Detection.

#include <cvl/cvl.h>
#include <math.h>

// Helper to generate your own images.
Image custom_image(void) {
    const int H = 256;
    const int W = 256;

    Image img = cvl_img_create_fill(H, W, BLACK);

    Point c1 = {64, 64};
    Point c2 = {192, 64};
    Point c3 = {64, 192};
    Point c4 = {192, 192};

    Point r_small = {20, 20};
    Point r_large = {30, 30};

    cvl_draw_ellipse_filled(img, c1, r_small, WHITE, WHITE, WHITE, WHITE);
    cvl_draw_ellipse_filled(img, c2, r_large, WHITE, WHITE, WHITE, WHITE);
    cvl_draw_ellipse_filled(img, c3, r_large, WHITE, WHITE, WHITE, WHITE);
    cvl_draw_ellipse_filled(img, c4, r_small, WHITE, WHITE, WHITE, WHITE);

    return img;
}

int main(void) {    
    // Load Input Image.
    // Image img = cvl_imread("./data/original/circles.pgm");
    // ... or you can make your own image with circles
    Image img = custom_image();
    if (!img.map) fprintf(stderr, "bad read\n");

    Matrix input = cvl_img2mat(img); // convert depth (u8 to f64)

    // Hough Circle Detection.
    bool inner = true; // detect inner vs. outer circle on 8-ball
    const double dp       = inner ?  1.0 : 1.0;
    const double min_dist = inner ?  40  : 20;
    const double thresh   = inner ?  15  : 10;
    const double canny_hi = inner ?  100 : 120;
    const int min_radius  = inner ?  15  : 40;
    const int max_radius  = inner ?  40  : 200;
    cvl_hough_circles_t circles = cvl_hough_circles_new(
        &input,
        dp,
        min_dist,
        canny_hi,
        thresh,
        min_radius,
        max_radius
    ); // hawk tuah

    // Save Results.
    printf("Found %zu circles.\n", circles.size);

    // Image circles_img = cvl_img_create_fill(img.height, img.width, BLACK);
    Image circles_img = cvl_img_copy(&img);
    cvl_draw_hough_circles(&circles_img, &circles);

    cvl_imwrite("./data/modified/1-original.ppm", &img);
    // todo: I think I'd rather pass in canny edges...
    cvl_imwrite("./data/modified/2-hough-circles.ppm", &circles_img);

    // Cleanup.
    cvl_hough_circles_free(&circles);
    cvl_img_free(circles_img);
    cvl_mat_free(input);
    cvl_img_free(img);

    return 0;
}
