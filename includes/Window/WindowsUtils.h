namespace Window
{
    class DPIScale
    {
    public:
        inline static float Scale = 1;
        static void Initialize(HWND hwnd)
        {
            Scale = 1.0f;
            float dpi = GetDpiForWindow(hwnd);
            Scale = dpi / 96.0f;
        }
    };
}