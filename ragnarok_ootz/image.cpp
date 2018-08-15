#include "image.h"

#include "window_manager.h"
#include "game_object_manager.h"

Image::Image()
{
    WindowManager::BindDC([this](HDC const& hdc) 
    {
        _hdc = CreateCompatibleDC(hdc);
        _hbit = (HBITMAP)CreateCompatibleBitmap(hdc, WindowInfo::kWidth, WindowInfo::kHeight);
        _holdbit = (HBITMAP)SelectObject(_hdc, _hbit);
    });

    BITMAP bit;
    GetObject(_hbit, sizeof(bit), &bit);
    _size.x = static_cast<float>(bit.bmWidth);
    _size.y = static_cast<float>(bit.bmHeight);

    _color_key = Color::kMagenta;
}

Image::Image(std::string const& path, COLORREF const& color_key)
{
    WindowManager::BindDC([&path, this](HDC const& hdc)
    {
        _hdc = CreateCompatibleDC(hdc);
        _hbit = (HBITMAP)LoadImage(WindowManager::GetInstance(), path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
        _holdbit = (HBITMAP)SelectObject(_hdc, _hbit);
    });

    BITMAP bit;
    GetObject(_hbit, sizeof(bit), &bit);
    _size.x = static_cast<float>(bit.bmWidth);
    _size.y = static_cast<float>(bit.bmHeight);

    _color_key = color_key;
}

Image::~Image()
{
    SelectObject(_hdc, _holdbit);
    DeleteObject(_hbit);
    DeleteDC(_hdc);
}

void Image::drawBlack()
{
    PatBlt(_hdc, 0, 0, static_cast<int>(_size.x), static_cast<int>(_size.y), BLACKNESS);
}

void Image::draw(HDC const& dest_dc, 
                 Vector2 const& dest_position)
{
    Draw(dest_dc, dest_position, _hdc, _size);
}

void Image::drawStretch(HDC const& dest_dc, 
                        Vector2 const& dest_position, 
                        Vector2 const& scale)
{
    DrawStretch(dest_dc, dest_position, _hdc, _size, scale);
}

void Image::drawTrans(HDC const& dest_dc, 
                      Vector2 const& dest_position)
{
    DrawTrans(dest_dc, dest_position, _hdc, _size, _color_key);
}

void Image::drawAlpha(HDC const& dest_dc, 
                      Vector2 const& dest_position, 
                      BYTE const& alpha)
{
    DrawAlpha(dest_dc, dest_position, _hdc, _size, alpha);
}

void Image::drawStretchTrans(HDC const& dest_dc, 
                             Vector2 const& dest_position, 
                             Vector2 const& scale)
{
    DrawStretchTrans(dest_dc, dest_position, _hdc, _size, scale, _color_key);
}

void Image::drawStretchAlpha(HDC const& dest_dc, 
                             Vector2 const& dest_position, 
                             Vector2 const& scale, 
                             BYTE const& alpha)
{
    DrawStretchAlpha(dest_dc, dest_position, _hdc, _size, scale, alpha);
}

void Image::drawTransAlpha(HDC const& dest_dc, 
                           Vector2 const& dest_position, 
                           BYTE const& alpha)
{
    DrawTransAlpha(dest_dc, dest_position, _hdc, _size, _color_key, alpha);
}

void Image::drawStretchTransAlpha(HDC const& dest_dc, 
                                  Vector2 const& dest_position, 
                                  Vector2 const& scale, 
                                  BYTE const& alpha)
{
    DrawStretchTransAlpha(dest_dc, dest_position, _hdc, _size, scale, _color_key, alpha);
}

void Image::drawSmart(HDC const& dest_dc, 
                      Vector2 const& dest_position, 
                      Vector2 const& scale, 
                      BYTE const& alpha,
                      float const& degrees)
{
    DrawSmart(dest_dc, dest_position + GameObjectManager::GetFactorOfPosition(), _hdc, _size, scale, _color_key, alpha, degrees);
}

Image::operator bool()
{
    if (!_hdc ||
        !_hbit ||
        !_holdbit)
        return false;

    return true;
}

HDC Image::getDC() const
{
    return _hdc;
}

Vector2 Image::getSize() const
{
    return _size;
}

COLORREF Image::getColorKey() const
{
    return _color_key;
}

void Image::Draw(HDC const& dest_dc, 
          Vector2 const& dest_position, 
          HDC const& source_dc, 
          Vector2 const& source_size)
{
    BitBlt(dest_dc, 
           static_cast<int>(dest_position.x - source_size.x / 2.0f), 
           static_cast<int>(dest_position.y - source_size.y / 2.0f), 
           static_cast<int>(source_size.x), 
           static_cast<int>(source_size.y), 
           source_dc, 
           0, 
           0, 
           SRCCOPY);
}

void Image::DrawStretch(HDC const& dest_dc, 
                        Vector2 const& dest_position, 
                        HDC const& source_dc, 
                        Vector2 const& source_size, 
                        Vector2 const& scale)
{
    float new_width = source_size.x * scale.x;
    float new_height = source_size.y * scale.y;

    StretchBlt(dest_dc,
               static_cast<int>(dest_position.x - new_width / 2.0f),
               static_cast<int>(dest_position.y - new_height / 2.0f),
               static_cast<int>(new_width),
               static_cast<int>(new_height),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);
}

void Image::DrawTrans(HDC const& dest_dc, 
                      Vector2 const& dest_position, 
                      HDC const & source_dc, 
                      Vector2 const & source_size, 
                      COLORREF const & color_key)
{
    TransparentBlt(dest_dc,
                   static_cast<int>(dest_position.x - source_size.x / 2.0f),
                   static_cast<int>(dest_position.y - source_size.y / 2.0f),
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y),
                   source_dc,
                   0,
                   0,
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y),
                   color_key);
}

void Image::DrawAlpha(HDC const& dest_dc, 
                      Vector2 const& dest_position, 
                      HDC const& source_dc, 
                      Vector2 const& source_size, 
                      BYTE const& alpha)
{
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;

    // alpha = 0(trans) ~ 255(opaque)
    bf.SourceConstantAlpha = alpha;
    bf.AlphaFormat = 0;
    AlphaBlend(dest_dc,
               static_cast<int>(dest_position.x - source_size.x / 2.0f),
               static_cast<int>(dest_position.y - source_size.y / 2.0f),
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               bf);
}

void Image::DrawRotate(HDC const& dest_dc, 
                       Vector2 const& dest_position,
                       HDC const& source_dc, 
                       Vector2 const& source_size, 
                       float const& degrees)
{
    float theta = Math::PI * degrees / 180.0f;
    Vector2 center = source_size * 0.5f;
    float cos = cosf(theta);
    float sin = sinf(theta);
    //float new_x = center_x + (x - center_x) * cosf(theta) - (y - center_y) * sinf(theta);
    //float new_y = center_y + (x - center_y) * sinf(theta) + (y - center_y) * cos(theta);
    POINT pt[3];
    pt[0].x = static_cast<LONG>(center.x + (0 - center.x) * cos - (0 - center.y) * sin);
    pt[0].y = static_cast<LONG>(center.y + (0 - center.x) * sin + (0 - center.y) * cos);
    pt[1].x = static_cast<LONG>(center.x + (source_size.x - center.x) * cos - (0 - center.y) * sin);
    pt[1].y = static_cast<LONG>(center.y + (source_size.x - center.x) * sin + (0 - center.y) * cos);
    pt[2].x = static_cast<LONG>(center.x + (0 - center.x) * cos - (source_size.y - center.y) * sin);
    pt[2].y = static_cast<LONG>(center.y + (0 - center.x) * sin + (source_size.y - center.y) * cos);

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(source_size.x), static_cast<int>(source_size.y));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    PlgBlt(dest_copy_dc, pt, source_dc, 0, 0, static_cast<int>(source_size.x), static_cast<int>(source_size.y), 0, 0, 0);

    Vector2 new_size = Vector2(2 * (std::abs(center.x - pt[0].x)), 2 * (std::abs(center.y - pt[1].y)));
    Draw(dest_dc, dest_position, dest_copy_dc, new_size);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawStretchTrans(HDC const& dest_dc, 
                             Vector2 const& dest_position, 
                             HDC const& source_dc, 
                             Vector2 const& source_size, 
                             Vector2 const& scale, 
                             COLORREF const& color_key)
{
    float dest_width = source_size.x * scale.x;
    float dest_height = source_size.y * scale.y;

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(dest_width), static_cast<int>(dest_height));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    StretchBlt(dest_copy_dc,
               0,
               0,
               static_cast<int>(dest_width),
               static_cast<int>(dest_height),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);

    DrawTrans(dest_dc, dest_position, dest_copy_dc, Vector2(dest_width, dest_height), color_key);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawStretchAlpha(HDC const& dest_dc, 
                             Vector2 const& dest_position, 
                             HDC const& source_dc, 
                             Vector2 const& source_size, 
                             Vector2 const & scale, 
                             BYTE const& alpha)
{
    float dest_width = source_size.x * scale.x;
    float dest_height = source_size.y * scale.y;

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(dest_width), static_cast<int>(dest_height));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    StretchBlt(dest_copy_dc,
               0,
               0,
               static_cast<int>(dest_width),
               static_cast<int>(dest_height),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);

    DrawAlpha(dest_dc, dest_position, dest_copy_dc, Vector2(dest_width, dest_height), alpha);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawTransAlpha(HDC const& dest_dc, 
                           Vector2 const& dest_position, 
                           HDC const& source_dc, 
                           Vector2 const& source_size, 
                           COLORREF const& color_key, 
                           BYTE const & alpha)
{
    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(source_size.x), static_cast<int>(source_size.y));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    BitBlt(dest_copy_dc, 
           0, 
           0, 
           static_cast<int>(source_size.x), 
           static_cast<int>(source_size.y), 
           dest_dc, 
           static_cast<int>(dest_position.x - source_size.x / 2.0f), 
           static_cast<int>(dest_position.y - source_size.y / 2.0f), 
           SRCCOPY);

    TransparentBlt(dest_copy_dc,
                   0,
                   0,
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y), 
                   source_dc, 
                   0, 
                   0, 
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y), 
                   color_key);

    DrawAlpha(dest_dc, dest_position, dest_copy_dc, source_size, alpha);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawRotateTrans(HDC const& dest_dc, 
                            Vector2 const& dest_position, 
                            HDC const& source_dc, 
                            Vector2 const& source_size, 
                            COLORREF const& color_key, 
                            float const& degrees)
{
    DrawTrans(dest_dc, dest_position, source_dc, source_size, color_key);
}

void Image::DrawStretchTransAlpha(HDC const& dest_dc, 
                                  Vector2 const& dest_position, 
                                  HDC const& source_dc, 
                                  Vector2 const& source_size, 
                                  Vector2 const& scale, 
                                  COLORREF const& color_key, 
                                  BYTE const& alpha)
{
    float dest_width = source_size.x * scale.x;
    float dest_height = source_size.y * scale.y;

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(dest_width), static_cast<int>(dest_height));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    StretchBlt(dest_copy_dc,
               0,
               0,
               static_cast<int>(dest_width),
               static_cast<int>(dest_height),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);

    HDC dest_copy_dc2 = CreateCompatibleDC(dest_copy_dc);
    HBITMAP hbit2 = CreateCompatibleBitmap(dest_copy_dc, static_cast<int>(dest_width), static_cast<int>(dest_height));
    HBITMAP holdbit2 = (HBITMAP)SelectObject(dest_copy_dc2, hbit2);

    BitBlt(dest_copy_dc2,
           0,
           0,
           static_cast<int>(dest_width),
           static_cast<int>(dest_height),
           dest_dc,
           static_cast<int>(dest_position.x - dest_width / 2.0f),
           static_cast<int>(dest_position.y - dest_height / 2.0f),
           SRCCOPY);

    TransparentBlt(dest_copy_dc2,
                   0,
                   0,
                   static_cast<int>(dest_width),
                   static_cast<int>(dest_height),
                   dest_copy_dc,
                   0,
                   0,
                   static_cast<int>(dest_width),
                   static_cast<int>(dest_height),
                   color_key);

    DrawAlpha(dest_dc, dest_position, dest_copy_dc2, Vector2(dest_width, dest_height), alpha);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);

    SelectObject(dest_copy_dc2, holdbit2);
    DeleteObject(hbit2);
    DeleteDC(dest_copy_dc2);
}

void Image::DrawStretchRotateTrans(HDC const& dest_dc, 
                                   Vector2 const& dest_position, HDC const& source_dc, 
                                   Vector2 const& source_size, 
                                   Vector2 const& scale, 
                                   COLORREF const& color_key, 
                                   float const& degrees)
{
    Vector2 dest_size = DotProduct(source_size, scale);

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(dest_size.x), static_cast<int>(dest_size.y));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    StretchBlt(dest_copy_dc,
               0,
               0,
               static_cast<int>(dest_size.x),
               static_cast<int>(dest_size.y),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);

    DrawTrans(dest_dc, dest_position, dest_copy_dc, Vector2(dest_size.x, dest_size.y), color_key);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawRotateTransAlpha(HDC const& dest_dc, 
                                 Vector2 const& dest_position, 
                                 HDC const& source_dc, 
                                 Vector2 const& source_size, 
                                 COLORREF const& color_key, 
                                 BYTE const& alpha, 
                                 float const& degrees)
{
    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(source_size.x), static_cast<int>(source_size.y));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    BitBlt(dest_copy_dc,
           0,
           0,
           static_cast<int>(source_size.x),
           static_cast<int>(source_size.y),
           dest_dc,
           static_cast<int>(dest_position.x - source_size.x / 2.0f),
           static_cast<int>(dest_position.y - source_size.y / 2.0f),
           SRCCOPY);

    TransparentBlt(dest_copy_dc,
                   0,
                   0,
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y),
                   dest_dc,
                   0,
                   0,
                   static_cast<int>(source_size.x),
                   static_cast<int>(source_size.y),
                   color_key);

    DrawAlpha(dest_dc, dest_position, dest_copy_dc, source_size, alpha);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);
}

void Image::DrawStretchRotateTransAlpha(HDC const& dest_dc, 
                                        Vector2 const& dest_position, 
                                        HDC const& source_dc, 
                                        Vector2 const& source_size, 
                                        Vector2 const& scale, 
                                        COLORREF const& color_key, 
                                        BYTE const& alpha, 
                                        float const& degrees)
{
    Vector2 dest_size = DotProduct(source_size, scale);

    HDC dest_copy_dc = CreateCompatibleDC(dest_dc);
    HBITMAP hbit = CreateCompatibleBitmap(dest_dc, static_cast<int>(dest_size.x), static_cast<int>(dest_size.y));
    HBITMAP holdbit = (HBITMAP)SelectObject(dest_copy_dc, hbit);

    StretchBlt(dest_copy_dc,
               0,
               0,
               static_cast<int>(dest_size.x),
               static_cast<int>(dest_size.y),
               source_dc,
               0,
               0,
               static_cast<int>(source_size.x),
               static_cast<int>(source_size.y),
               SRCCOPY);

    HDC dest_copy_dc2 = CreateCompatibleDC(dest_copy_dc);
    HBITMAP hbit2 = CreateCompatibleBitmap(dest_copy_dc, static_cast<int>(dest_size.x), static_cast<int>(dest_size.y));
    HBITMAP holdbit2 = (HBITMAP)SelectObject(dest_copy_dc2, hbit2);

    BitBlt(dest_copy_dc2,
           0,
           0,
           static_cast<int>(dest_size.x),
           static_cast<int>(dest_size.y),
           dest_dc,
           static_cast<int>(dest_position.x - dest_size.x / 2.0f),
           static_cast<int>(dest_position.y - dest_size.y / 2.0f),
           SRCCOPY);

    TransparentBlt(dest_copy_dc2,
                   0,
                   0,
                   static_cast<int>(dest_size.x),
                   static_cast<int>(dest_size.y),
                   dest_copy_dc,
                   0,
                   0,
                   static_cast<int>(dest_size.x),
                   static_cast<int>(dest_size.y),
                   color_key);

    DrawAlpha(dest_dc, dest_position, dest_copy_dc2, dest_size, alpha);

    SelectObject(dest_copy_dc, holdbit);
    DeleteObject(hbit);
    DeleteDC(dest_copy_dc);

    SelectObject(dest_copy_dc2, holdbit2);
    DeleteObject(hbit2);
    DeleteDC(dest_copy_dc2);
}

void Image::DrawSmart(HDC const& dest_dc, 
                      Vector2 const& dest_position, 
                      HDC const& source_dc, 
                      Vector2 const& source_size, 
                      Vector2 const& scale, 
                      COLORREF const& color_key, 
                      BYTE const& alpha,
                      float const& degrees)
{
    auto size = DotProduct(source_size, scale);
    auto half_width = size.x / 2.0f;
    auto half_height = size.y / 2.0f;
    if (dest_position.x - half_width > WindowInfo::kWidth ||
        dest_position.x + half_width < 0 ||
        dest_position.y - half_height > WindowInfo::kHeight ||
        dest_position.y + half_height < 0)
        return;

    if (scale == Vector2::One() && alpha == 255 && degrees == 0.0f)
        DrawTrans(dest_dc, dest_position, source_dc, source_size, color_key);
    else if (scale == Vector2::One() && alpha == 255)
        DrawRotateTrans(dest_dc, dest_position, source_dc, source_size, color_key, degrees);
    else if (scale == Vector2::One() && degrees == 0.0f)
        DrawTransAlpha(dest_dc, dest_position, source_dc, source_size, color_key, alpha);
    else if (alpha == 255 && degrees == 0.0f)
        DrawStretchTrans(dest_dc, dest_position, source_dc, source_size, scale, color_key);
    else if (scale == Vector2::One())
        DrawRotateTransAlpha(dest_dc, dest_position, source_dc, source_size, color_key, alpha, degrees);
    else if (alpha == 255)
        DrawStretchRotateTrans(dest_dc, dest_position, source_dc, source_size, scale, color_key, degrees);
    else if (degrees == 0.0f)
        DrawStretchTransAlpha(dest_dc, dest_position, source_dc, source_size, scale, color_key, alpha);
    else
        DrawStretchRotateTransAlpha(dest_dc, dest_position, source_dc, source_size, scale, color_key, alpha, degrees);
}
