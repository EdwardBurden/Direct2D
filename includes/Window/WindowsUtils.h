namespace Window
{
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
    };
}