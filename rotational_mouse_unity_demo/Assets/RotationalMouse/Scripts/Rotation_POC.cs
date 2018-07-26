using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RotationalMouseDemo
{
    public class Rotation_POC : MonoBehaviour
    {
        [SerializeField]
        private Transform _rotationTarget;

        [SerializeField]
        private RotationalMouseTest _mouse;
        
        [SerializeField]
        private Vector3 _rotationAmplication = new Vector3(8f, 8f, 20f);

        //Constrain Rotation to a single axis:
        private Vector3 _rotationAxis;

        private void Update()
        {
            if(Input.GetKey(KeyCode.Mouse0))
            {
                UpdateRotation();
            }
        }

        private void UpdateRotation()
        {
            Vector3 mouseRotation = new Vector3(_mouse.MouseX, _mouse.MouseY, _mouse.MouseRotation);
            Vector3 scaledRotation = Vector3.Scale(mouseRotation, _rotationAmplication);


            float rotationIntensity = SetRotationAxis(scaledRotation);
            _rotationTarget.RotateAround(_rotationTarget.position, _rotationAxis, rotationIntensity * Time.deltaTime);

        }

        private float SetRotationAxis(Vector3 scaledRotation)
        {
            float largest = Mathf.Abs(scaledRotation.x);
            _rotationAxis = -Mathf.Sign(scaledRotation.x) * Vector3.up;
            if (Mathf.Abs(scaledRotation.y) > largest)
            {
                largest = Mathf.Abs(scaledRotation.y);
                _rotationAxis = Mathf.Sign(scaledRotation.y) * Vector3.right;
            }
            if (Mathf.Abs(scaledRotation.z) > largest)
            {
                largest = Mathf.Abs(scaledRotation.z);
                _rotationAxis = Mathf.Sign(scaledRotation.z) * Vector3.forward;
            }
            return largest;
        }
    }
}