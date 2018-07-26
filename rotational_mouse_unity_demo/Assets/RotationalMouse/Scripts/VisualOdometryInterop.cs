using UnityEngine;
using System.Runtime.InteropServices;

namespace RotationalMouseDemo
{
    public class VisualOdometryInterop : MonoBehaviour
    {
        public float Angle
        {
            get; private set;
        }
        public float X
        {
            get; private set;
        }
        public float Y
        {
            get; private set;
        }

        private const float MAX_ANGLE = 4f;

        [DllImport("open_cv_test", EntryPoint = "InitializeCamera")]
        public static extern void initialize_camera();

        [DllImport("open_cv_test", EntryPoint = "UpdateCamera")]
        public static extern void update_camera();

        [DllImport("open_cv_test", EntryPoint = "GetRotation")]
        public static extern double get_rotation();

        [DllImport("open_cv_test", EntryPoint = "XTranslation")]
        public static extern double get_translation_x();

        [DllImport("open_cv_test", EntryPoint = "YTranslation")]
        public static extern double get_translation_y();

        private void Start()
        {
            initialize_camera();
        }

        private void Update()
        {
            update_camera();
            Angle = GetAdjustedAngle(get_rotation());
            X = (float)get_translation_x();
            Y = -(float)get_translation_y();
        }

        private float GetAdjustedAngle(double angle)
        {
            float rangeAdjustedAngle = (angle > 180d) ? ((float)angle - 360f) : ((float)angle);
            float cappedAngle = (Mathf.Abs(rangeAdjustedAngle) <= MAX_ANGLE) ? rangeAdjustedAngle : 0f;
            return cappedAngle;
        }
    }
}
