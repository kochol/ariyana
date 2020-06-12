using System;

namespace ari
{
	public static class NetSerializer
	{
		// bool
		[CLink]
		static extern bool SerializeBool(void* _stream, bool* _val);
		[CLink]
		static extern bool SerializeMeasureBool(void* _stream, bool* _val);
		[CLink]
		static extern bool DeserializeBool(void* _stream, bool* _val);
		
		// int32
		[CLink]
		static extern bool SerializeInt32(void* _stream, int* _val);
		[CLink]
		static extern bool SerializeMeasureInt32(void* _stream, int* _val);
		[CLink]
		static extern bool DeserializeInt32(void* _stream, int* _val);
		
		// uint32
		[CLink]
		static extern bool SerializeUint32(void* _stream, uint32* _val);
		[CLink]
		static extern bool SerializeMeasureUint32(void* _stream, uint32* _val);
		[CLink]
		static extern bool DeserializeUint32(void* _stream, uint32* _val);
		
		// uint64
		[CLink]
		static extern bool SerializeUint64(void* _stream, uint64* _val);
		[CLink]
		static extern bool SerializeMeasureUint64(void* _stream, uint64* _val);
		[CLink]
		static extern bool DeserializeUint64(void* _stream, uint64* _val);
		
		// float
		[CLink]
		static extern bool SerializeFloat(void* _stream, float* _val);
		[CLink]
		static extern bool SerializeMeasureFloat(void* _stream, float* _val);
		[CLink]
		static extern bool DeserializeFloat(void* _stream, float* _val);
		
		// double
		[CLink]
		static extern bool SerializeDouble(void* _stream, double* _val);
		[CLink]
		static extern bool SerializeMeasureDouble(void* _stream, double* _val);
		[CLink]
		static extern bool DeserializeDouble(void* _stream, double* _val);
		
		// string
		[CLink]
		static extern bool SerializeString(void* _stream, char8* _val, int32 _size);
		[CLink]
		static extern bool SerializeMeasureString(void* _stream, char8* _val, int32 _size);
		[CLink]
		static extern bool DeserializeString(void* _stream, char8* _val, int32 _size);
		
		// bits
		[CLink]
		static extern bool SerializeBits(void* _stream, void* _val, int32 _size);
		[CLink]
		static extern bool SerializeMeasureBits(void* _stream, void* _val, int32 _size);
		[CLink]
		static extern bool DeserializeBits(void* _stream, void* _val, int32 _size);
		
		// bytes
		[CLink]
		static extern bool SerializeBytes(void* _stream, void* _val, int32 _size);
		[CLink]
		static extern bool SerializeMeasureBytes(void* _stream, void* _val, int32 _size);
		[CLink]
		static extern bool DeserializeBytes(void* _stream, void* _val, int32 _size);
		
		// Generic
		public static bool Serialize<T>(void* _stream, T _val)
		{
			var t = _val.GetType();
			var c = _val;
			switch (t.[Friend]mTypeCode)
			{
				case .Boolean: return SerializeBool(_stream, (bool*)&c);

				case .Char8, .Int8, .UInt8: return SerializeBits(_stream, &c, 8);

				case .Char16, .Int16, .UInt16: return SerializeBits(_stream, &c, 16);

				case .Int, .Char32, .Int32, .UInt, .UInt32: return SerializeUint32(_stream, (uint32*)&c);

				case .Int64, .UInt64: return SerializeUint64(_stream, (uint64*)&c);

				case .Float: return SerializeFloat(_stream, (float*)&c);

				case .Double: return SerializeDouble(_stream, (double*)&c);

				case .Enum: return SerializeBits(_stream, &c, t.Size * 8);

				case .CharPtr:
					char8* str = (char8*)Internal.UnsafeCastToPtr(c);
					return SerializeString(_stream, str, String.StrLen(str));

				case .Struct: return SerializeBytes(_stream, Internal.UnsafeCastToPtr(c), t.Size);

				default:
				Runtime.Assert(false, "Unsupported type: ");
			}

			return false;
		}

		public static bool SerializeMeasure<T>(void* _stream, T _val)
		{
			var t = _val.GetType();
			var c = _val;
			switch (t.[Friend]mTypeCode)
			{
				case .Boolean: return SerializeMeasureBool(_stream, (bool*)&c);

				case .Char8, .Int8, .UInt8: return SerializeMeasureBits(_stream, &c, 8);
				
				case .Char16, .Int16, .UInt16: return SerializeMeasureBits(_stream, &c, 16);

				case .Int, .Char32, .Int32, .UInt, .UInt32: return SerializeMeasureUint32(_stream, (uint32*)&c);
				
				case .Int64, .UInt64: return SerializeMeasureUint64(_stream, (uint64*)&c);

				case .Float: return SerializeMeasureFloat(_stream, (float*)&c);

				case .Double: return SerializeMeasureDouble(_stream, (double*)&c);

				case .Enum: return SerializeMeasureBits(_stream, &c, t.Size * 8);

				case .CharPtr:
					char8* str = (char8*)Internal.UnsafeCastToPtr(c);
					return SerializeMeasureString(_stream, str, String.StrLen(str));

				case .Struct: return SerializeMeasureBytes(_stream, Internal.UnsafeCastToPtr(c), t.Size);

				default:
				Runtime.Assert(false, "Unsupported type: ");
			}

			return false;
		}

		public static bool Deserialize<T>(void* _stream, ref T _val)
		{
			var t = _val.GetType();
			var c = _val;
			bool r = false;
			switch (t.[Friend]mTypeCode)
			{
				case .Boolean: r = DeserializeBool(_stream, (bool*)&c);

				case .Char8, .Int8, .UInt8:r = DeserializeBits(_stream, &c, 8);
				
				case .Char16, .Int16, .UInt16: r = DeserializeBits(_stream, &c, 16);
				
				case .Int, .Char32, .Int32, .UInt, .UInt32: r = DeserializeUint32(_stream, (uint32*)&c);
				
				case .Int64,.UInt64: r = DeserializeUint64(_stream, (uint64*)&c);

				case .Float: r = DeserializeFloat(_stream, (float*)&c);

				case .Double: r = DeserializeDouble(_stream, (double*)&c);

				case .Enum: r = DeserializeBits(_stream, &c, t.Size * 8);

				case .CharPtr:
					char8* str = (char8*)Internal.UnsafeCastToPtr(c);
					r = DeserializeString(_stream, str, String.StrLen(str));

				case .Struct: r = DeserializeBytes(_stream, Internal.UnsafeCastToPtr(c), t.Size);

				default:
				Runtime.Assert(false, "Unsupported type: ");
			}
			if (r)
				_val = c;
			return r;
		}
	}
}
