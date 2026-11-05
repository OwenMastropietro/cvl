#include <cvl/core.h>
#include <cvl/error.h>

bool cvl_mat_ok(const cvl_Mat *mat) {
    return mat && mat->data && mat->height >= 0 && mat->width >= 0 && mat->channels >= 0;
}

bool cvl_mat_same_size(const cvl_Mat *a, cvl_Mat *b) {
    return a->height == b->height && a->width == b->width;
}

bool cvl_mat_same_shape(const cvl_Mat *a, cvl_Mat *b) {
    return cvl_mat_same_size(a, b) && a->channels == b->channels;
}
