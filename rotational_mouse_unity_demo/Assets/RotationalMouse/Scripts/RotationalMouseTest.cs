using UnityEngine;

namespace RotationalMouseDemo
{
    public class RotationalMouseTest : MonoBehaviour
    {
        private VisualOdometryInterop _visual_odometer;

        private GameObject _cursor;

        [SerializeField]
        private float TRANSLATION_AMP = 0.05f;
        [SerializeField]
        private float ROTATIONAL_AMP = 2f;

        [SerializeField]
        private float TRANSLATION_SMOOTHING = 0.75f;
        [SerializeField]
        private float ROTATIONAL_SMOOTHING = 0.9f;

        [SerializeField]
        private bool _useStdMouseXY = true;//Using standard mouse for translation

        private float _priorX;
        private float _priorY;
        private float _priorRotation;

        private const float MAX_X = 7.5f;
        private const float MAX_Y = 4f;

        public float MouseX
        {
            get; private set;
        }

        public float MouseY
        {
            get; private set;
        }

        public float MouseRotation
        {
            get; private set;
        }

        // Use this for initialization
        private void Start()
        {
            if(_useStdMouseXY)
            {
                _priorX = Input.mousePosition.x;
                _priorY = Input.mousePosition.y;
            }
            _visual_odometer = this.gameObject.AddComponent<VisualOdometryInterop>();
            _cursor = this.gameObject;
        }

        // Update is called once per frame
        private void Update()
        {
            GetNewMousePosition();
            UpdateCursor();
        }

        private void GetNewMousePosition()
        {
            if (_useStdMouseXY)
            {
                MouseX = Input.mousePosition.x - _priorX;
                _priorX = Input.mousePosition.x;
                MouseY = Input.mousePosition.y - _priorY;
                _priorY = Input.mousePosition.y;
            }
            else
            {
                MouseX = SmoothUpdate(_priorX, _visual_odometer.X, TRANSLATION_SMOOTHING);
                _priorX = MouseX;

                MouseY = SmoothUpdate(_priorY, _visual_odometer.Y, TRANSLATION_SMOOTHING);
                _priorY = MouseY;
            }

            MouseRotation = SmoothUpdate(_priorRotation, _visual_odometer.Angle, ROTATIONAL_SMOOTHING);
            _priorRotation = MouseRotation;
        }

        private float SmoothUpdate(float previousVal, float currentVal, float smoothing)
        {
            return smoothing * previousVal + (1 - smoothing) * currentVal;
        }

        private void UpdateCursor()
        {
            Vector3 newTranslation = new Vector3(Mathf.Clamp(_cursor.transform.position.x + TRANSLATION_AMP * MouseX, -MAX_X, MAX_X),
                Mathf.Clamp(_cursor.transform.position.y + TRANSLATION_AMP * MouseY, -MAX_Y, MAX_Y), 0f);

            _cursor.transform.position = newTranslation;
            _cursor.transform.eulerAngles += ROTATIONAL_AMP * (new Vector3(0f, 0f, MouseRotation));
        }
    }
}