/*
 * MIT License
 *
 * Copyright (c) 2026 DerBallo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

inline constexpr long double deg_to_rad_ld { 0.017453292519943295769236907684886_ld };
inline constexpr double deg_to_rad_d { 0.017453292519943295769236907684886_d };
inline constexpr float deg_to_rad_f { 0.017453292519943295769236907684886_f };
inline constexpr long double rad_to_deg_ld { 57.295779513082320876798154814105_ld };
inline constexpr double rad_to_deg_d { 57.295779513082320876798154814105_d };
inline constexpr float rad_to_deg_f { 57.295779513082320876798154814105_f };
inline constexpr long double pi_ld { 3.1415926535897932384626433832795_ld };
inline constexpr double pi_d { 3.1415926535897932384626433832795_d };
inline constexpr float pi_f { 3.1415926535897932384626433832795_f };
inline constexpr long double sphere_factor_ld { 0.62035049089940001666800681204778_ld };
inline constexpr double sphere_factor_d { 0.62035049089940001666800681204778_d };
inline constexpr float sphere_factor_f { 0.62035049089940001666800681204778_f };

constexpr float fov_to_focal_distance_f(float fov) noexcept
{
    return 0.5_f / std::tan(fov * 0.5_f * deg_to_rad_f);
}

constexpr double fov_to_focal_distance_d(double fov) noexcept
{
    return 0.5_d / std::tan(fov * 0.5_d * deg_to_rad_d);
}

constexpr long double fov_to_focal_distance_ld(long double fov) noexcept
{
    return 0.5_ld / std::tan(fov * 0.5_ld * deg_to_rad_ld);
}
