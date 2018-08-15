#pragma once

#include <Windows.h>
#include <string>

#include "vector2.h"
#include "utility.h"

class Image
{
public:
    Image();
    Image(std::string const& path, COLORREF const& color_key = Color::kMagenta);
    ~Image();

    void drawBlack();
    void draw(HDC const& dest_dc, 
              Vector2 const& dest_position);
    void drawStretch(HDC const& dest_dc, 
                     Vector2 const& dest_position, 
                     Vector2 const& scale);
    void drawTrans(HDC const& dest_dc, 
                   Vector2 const& dest_position);
    void drawAlpha(HDC const& dest_dc, 
                   Vector2 const& dest_position, 
                   BYTE const& alpha);
    void drawStretchTrans(HDC const& dest_dc, 
                          Vector2 const& dest_position, 
                          Vector2 const& scale);
    void drawStretchAlpha(HDC const& dest_dc, 
                          Vector2 const& dest_position, 
                          Vector2 const& scale, 
                          BYTE const& alpha);
    void drawTransAlpha(HDC const& dest_dc, 
                        Vector2 const& dest_position, 
                        BYTE const& alpha);
    void drawStretchTransAlpha(HDC const& dest_dc, 
                               Vector2 const& dest_position, 
                               Vector2 const& scale, 
                               BYTE const& alpha);
    void drawSmart(HDC const& dest_dc,
                   Vector2 const& dest_position,
                   Vector2 const& scale = Vector2::one,
                   BYTE const& alpha = 255,
                   float const& degrees = 0.0f);

    operator bool();

    HDC getDC() const;
    Vector2 getSize() const;
    COLORREF getColorKey() const;

    static void Draw(HDC const& dest_dc,
                     Vector2 const& dest_position,
                     HDC const& source_dc,
                     Vector2 const& source_size);
    static void DrawStretch(HDC const& dest_dc,
                            Vector2 const& dest_position,
                            HDC const& source_dc,
                            Vector2 const& source_size,
                            Vector2 const& scale);
    static void DrawTrans(HDC const& dest_dc,
                          Vector2 const& dest_position,
                          HDC const& source_dc,
                          Vector2 const& source_size,
                          COLORREF const& color_key);
    static void DrawAlpha(HDC const& dest_dc,
                          Vector2 const& dest_position,
                          HDC const& source_dc,
                          Vector2 const& source_size,
                          BYTE const& alpha);
    static void DrawRotate(HDC const& dest_dc,
                           Vector2 const& dest_position,
                           HDC const& source_dc,
                           Vector2 const& source_size,
                           float const& degrees);
    static void DrawStretchTrans(HDC const& dest_dc,
                                 Vector2 const& dest_position,
                                 HDC const& source_dc,
                                 Vector2 const& source_size,
                                 Vector2 const& scale,
                                 COLORREF const& color_key);
    static void DrawStretchAlpha(HDC const& dest_dc,
                                 Vector2 const& dest_position,
                                 HDC const& source_dc,
                                 Vector2 const& source_size,
                                 Vector2 const& scale,
                                 BYTE const& alpha);
    static void DrawTransAlpha(HDC const& dest_dc,
                               Vector2 const& dest_position,
                               HDC const& source_dc,
                               Vector2 const& source_size,
                               COLORREF const& color_key,
                               BYTE const& alpha);
    static void DrawRotateTrans(HDC const& dest_dc,
                                Vector2 const& dest_position,
                                HDC const& source_dc,
                                Vector2 const& source_size,
                                COLORREF const& color_key,
                                float const& degrees);
    static void DrawStretchTransAlpha(HDC const& dest_dc,
                                      Vector2 const& dest_position,
                                      HDC const& source_dc,
                                      Vector2 const& source_size,
                                      Vector2 const& scale,
                                      COLORREF const& color_key,
                                      BYTE const& alpha);
    static void DrawStretchRotateTrans(HDC const& dest_dc,
                                       Vector2 const& dest_position,
                                       HDC const& source_dc,
                                       Vector2 const& source_size,
                                       Vector2 const& scale,
                                       COLORREF const& color_key,
                                       float const& degrees);
    static void DrawRotateTransAlpha(HDC const& dest_dc,
                                     Vector2 const& dest_position,
                                     HDC const& source_dc,
                                     Vector2 const& source_size,
                                     COLORREF const& color_key,
                                     BYTE const& alpha,
                                     float const& degrees);
    static void DrawStretchRotateTransAlpha(HDC const& dest_dc,
                                            Vector2 const& dest_position,
                                            HDC const& source_dc,
                                            Vector2 const& source_size,
                                            Vector2 const& scale,
                                            COLORREF const& color_key,
                                            BYTE const& alpha,
                                            float const& degrees);
    static void DrawSmart(HDC const& dest_dc,
                          Vector2 const& dest_position,
                          HDC const& source_dc,
                          Vector2 const& source_size,
                          Vector2 const& scale,
                          COLORREF const& color_key,
                          BYTE const& alpha,
                          float const& degrees);

private:
    HDC _hdc;
    HBITMAP _hbit;
    HBITMAP _holdbit;
    Vector2 _size;
    COLORREF _color_key;
};

