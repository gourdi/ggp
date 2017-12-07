#include <ggo_nonreg.h>
#include <ggo_image_processing.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(image_processing, view_pixel_rect)
{
  {
    const int image_width = 4;
    const int image_height = 3;
    const int view_width = 12;
    const int view_height = 6;

    const auto basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);

    auto rect1 = ggo::get_view_pixel_rect(basis, { 0, 0 });
    GGO_CHECK_FLOAT_EQ(rect1._pos.x(), -1.5f);
    GGO_CHECK_FLOAT_EQ(rect1._pos.y(), -0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._width, 0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._height, 0.5f);

    auto rect2 = ggo::get_view_pixel_rect(basis, { 11, 5 });
    GGO_CHECK_FLOAT_EQ(rect2._pos.x(), 4.0f);
    GGO_CHECK_FLOAT_EQ(rect2._pos.y(), 2.0f);
    GGO_CHECK_FLOAT_EQ(rect2._width, 0.5f);
    GGO_CHECK_FLOAT_EQ(rect2._height, 0.5f);
  }

  {
    const int image_width = 4;
    const int image_height = 3;
    const int view_width = 8;
    const int view_height = 10;

    const auto basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);

    auto rect1 = ggo::get_view_pixel_rect(basis, { 0, 0 });
    GGO_CHECK_FLOAT_EQ(rect1._pos.x(), -0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._pos.y(), -1.5f);
    GGO_CHECK_FLOAT_EQ(rect1._width, 0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._height, 0.5f);

    auto rect2 = ggo::get_view_pixel_rect(basis, { 7, 9 });
    GGO_CHECK_FLOAT_EQ(rect2._pos.x(), 3.0f);
    GGO_CHECK_FLOAT_EQ(rect2._pos.y(), 3.0f);
    GGO_CHECK_FLOAT_EQ(rect2._width, 0.5f);
    GGO_CHECK_FLOAT_EQ(rect2._height, 0.5f);
  }

  {
    const int image_width = 8;
    const int image_height = 6;
    const int view_width = 6;
    const int view_height = 3;

    const auto basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);

    auto rect1 = ggo::get_view_pixel_rect(basis, { 0, 0 });
    GGO_CHECK_FLOAT_EQ(rect1._pos.x(), -2.5f);
    GGO_CHECK_FLOAT_EQ(rect1._pos.y(), -0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._width, 2.0f);
    GGO_CHECK_FLOAT_EQ(rect1._height, 2.0f);

    auto rect2 = ggo::get_view_pixel_rect(basis, { 5, 2 });
    GGO_CHECK_FLOAT_EQ(rect2._pos.x(), 7.5f);
    GGO_CHECK_FLOAT_EQ(rect2._pos.y(), 3.5f);
    GGO_CHECK_FLOAT_EQ(rect2._width, 2.0f);
    GGO_CHECK_FLOAT_EQ(rect2._height, 2.0f);
  }

  {
    const int image_width = 8;
    const int image_height = 6;
    const int view_width = 4;
    const int view_height = 5;

    const auto basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);

    auto rect1 = ggo::get_view_pixel_rect(basis, { 0, 0 });
    GGO_CHECK_FLOAT_EQ(rect1._pos.x(), -0.5f);
    GGO_CHECK_FLOAT_EQ(rect1._pos.y(), -2.5f);
    GGO_CHECK_FLOAT_EQ(rect1._width, 2.0f);
    GGO_CHECK_FLOAT_EQ(rect1._height, 2.0f);

    auto rect2 = ggo::get_view_pixel_rect(basis, { 3, 4 });
    GGO_CHECK_FLOAT_EQ(rect2._pos.x(), 5.5f);
    GGO_CHECK_FLOAT_EQ(rect2._pos.y(), 5.5f);
    GGO_CHECK_FLOAT_EQ(rect2._width, 2.0f);
    GGO_CHECK_FLOAT_EQ(rect2._height, 2.0f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image_processing, view_basis)
{
  {
    const int image_width = 4;
    const int image_height = 3;
    const int view_width = 12;
    const int view_height = 6;

    auto view_basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);
    GGO_CHECK_FLOAT_EQ(view_basis.x().x(), 0.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().y(), 0.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().x(), -1.25f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().y(), -0.25f);
  }

  {
    const int image_width = 4;
    const int image_height = 3;
    const int view_width = 8;
    const int view_height = 10;

    auto view_basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);
    GGO_CHECK_FLOAT_EQ(view_basis.x().x(), 0.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().y(), 0.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().x(), -0.25f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().y(), -1.25f);
  }

  {
    const int image_width = 8;
    const int image_height = 6;
    const int view_width = 6;
    const int view_height = 3;

    auto view_basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);
    GGO_CHECK_FLOAT_EQ(view_basis.x().x(), 2.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().y(), 2.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().x(), -1.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().y(), 0.5f);
  }

  {
    const int image_width = 8;
    const int image_height = 6;
    const int view_width = 4;
    const int view_height = 5;

    auto view_basis = ggo::compute_fit_view_basis(image_width, image_height, view_width, view_height);
    GGO_CHECK_FLOAT_EQ(view_basis.x().x(), 2.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.y().y(), 2.0f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().x(), 0.5f);
    GGO_CHECK_FLOAT_EQ(view_basis.pos().y(), -1.5f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image_processing, clamp_view_basis)
{
  {
    auto clamped_basis = ggo::clamp_basis_view(3, 2, 3, 2, ggo::basis2d<float>({ 5.f, 5.f }, { 0.25f, 0.f }, { 0.f, 0.25f }));
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().x(), 1.875f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().y(), 1.125f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().x(), 0.25f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().y(), 0.25f);
  }

  {
    auto clamped_basis = ggo::clamp_basis_view(3, 2, 3, 2, ggo::basis2d<float>({ -5.f, -5.f }, { 0.25f, 0.f }, { 0.f, 0.25f }));
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().x(), -0.375f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().y(), -0.375f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().x(), 0.25f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().y(), 0.25f);
  }

  {
    auto clamped_basis = ggo::clamp_basis_view(4, 2, 5, 2, ggo::basis2d<float>({ 0.f, 0.f }, { 2.f, 0.f }, { 0.f, 2.f }));
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().x(), -2.5f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.pos().y(), -0.5f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().x(), 2.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.x().y(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(clamped_basis.y().y(), 2.0f);
  }
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  RUN_ALL_TESTS(argc, argv);

  return 0;
}
