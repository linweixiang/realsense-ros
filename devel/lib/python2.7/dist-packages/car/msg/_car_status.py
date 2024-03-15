# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from car/car_status.msg. Do not edit."""
import codecs
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class car_status(genpy.Message):
  _md5sum = "4708c50435eab9caae1824db89482659"
  _type = "car/car_status"
  _has_header = False  # flag to mark the presence of a Header object
  _full_text = """uint64 time
uint8 status
float32 px
float32 py
float32 pz
float32 vx
float32 vy
float32 vz
float32 yaw
float32 pitch
float32 roll
"""
  __slots__ = ['time','status','px','py','pz','vx','vy','vz','yaw','pitch','roll']
  _slot_types = ['uint64','uint8','float32','float32','float32','float32','float32','float32','float32','float32','float32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       time,status,px,py,pz,vx,vy,vz,yaw,pitch,roll

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(car_status, self).__init__(*args, **kwds)
      # message fields cannot be None, assign default values for those that are
      if self.time is None:
        self.time = 0
      if self.status is None:
        self.status = 0
      if self.px is None:
        self.px = 0.
      if self.py is None:
        self.py = 0.
      if self.pz is None:
        self.pz = 0.
      if self.vx is None:
        self.vx = 0.
      if self.vy is None:
        self.vy = 0.
      if self.vz is None:
        self.vz = 0.
      if self.yaw is None:
        self.yaw = 0.
      if self.pitch is None:
        self.pitch = 0.
      if self.roll is None:
        self.roll = 0.
    else:
      self.time = 0
      self.status = 0
      self.px = 0.
      self.py = 0.
      self.pz = 0.
      self.vx = 0.
      self.vy = 0.
      self.vz = 0.
      self.yaw = 0.
      self.pitch = 0.
      self.roll = 0.

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_QB9f().pack(_x.time, _x.status, _x.px, _x.py, _x.pz, _x.vx, _x.vy, _x.vz, _x.yaw, _x.pitch, _x.roll))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 45
      (_x.time, _x.status, _x.px, _x.py, _x.pz, _x.vx, _x.vy, _x.vz, _x.yaw, _x.pitch, _x.roll,) = _get_struct_QB9f().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_QB9f().pack(_x.time, _x.status, _x.px, _x.py, _x.pz, _x.vx, _x.vy, _x.vz, _x.yaw, _x.pitch, _x.roll))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 45
      (_x.time, _x.status, _x.px, _x.py, _x.pz, _x.vx, _x.vy, _x.vz, _x.yaw, _x.pitch, _x.roll,) = _get_struct_QB9f().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_QB9f = None
def _get_struct_QB9f():
    global _struct_QB9f
    if _struct_QB9f is None:
        _struct_QB9f = struct.Struct("<QB9f")
    return _struct_QB9f
