class DPIScale
{
    static float scale;

public:
    static void Initialize(HWND hwnd)
    {
        scale = 1.0f;
        float dpi = GetDpiForWindow(hwnd);
        scale = dpi / 96.0f;
    }

    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y)
    {
        return D2D1::Point2F(static_cast<float>(x) / scale, static_cast<float>(y) / scale);
    }
};