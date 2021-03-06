﻿#include "DarkSkinIncludes.h"

namespace vl
{
	namespace presentation
	{
		namespace user_resource
		{
			using namespace collections;
			using namespace stream;
			using namespace controls;

			class DarkSkinResourceReader
			{
			public:
				static const vint parserBufferLength = 1702; // 4359 bytes before compressing
				static const vint parserBufferBlock = 1024;
				static const vint parserBufferRemain = 678;
				static const vint parserBufferRows = 2;
				static const char* parserBuffer[2];

				static void ReadToStream(vl::stream::MemoryStream& stream)
				{
					DecompressStream(parserBuffer, true, parserBufferRows, parserBufferBlock, parserBufferRemain, stream);
				}
			};

			const char* DarkSkinResourceReader::parserBuffer[] = {
				"\x07\x11\x00\x00\x9E\x06\x00\x00\x52\x00\x01\x82\x80\x00\x81\x80\x1E\x29\x65\x73\x2F\x35\x32\x3B\x31\x33\x26\x65\x74\x21\x24\x31\x32\x88\x10\x27\x61\x6D\x25\x3D\x12\x14\x21\x30\x39\x6B\x53\x2B\x29\x3E\x32\x10\x10\x2B\x65\x72\x33\x29\x3F\x36\x34\x8F\x18\x2E\x30\x22\x1E\x1C\x1C\x21\x32\x38\x65\x6E\x24\x39\x93\x31\x35\x85\x17\x34\xAF\x08\x8A\x8C\x86\x84\x88\x89\x14\x92\xBE\x05\x03\x82\x80\x05\xA6\x01\xCF\x81\x89\x2E\x33\x3A\x88\x37\x0E\x8D\x10\xA0\x0A\xAB\xA9\xAB\xAB\x0E\xD3\x34\x39\x3C\x35\x33\x01\xAD\x67\x81\x84\x2F\x33\x34\x03\xAC\xB7\x01\xD4\x25\x38\x34\x3F\x07\xAC\xBB\x01\xC3\x2C\x21\x33\x3B\x38\x8C\x8D\x08\xE3\x2F\x32\x34\x33\xAC\x01\xB4\x4D\xD2\x89\xC2\x8E\x88\x92\x90\x92\x86\x86\xCA\x1B\xA6\x04\xCA\x80\x21\x75\x74\x34\x2D\x91\xCD\xCF\xC4\xA8\x8A\xEB\xA3\x35\x3A\x8E\x34\x3A\xC3\x01\x5B\x82\x1B\xA0\x0C\xD7\xCA\xBE\x65\x6C\x24\x85\x32\xD4\xD1\x9C\x3A\xBE\x5B\xB9\xC6\x82\xD4\x9B\x21\x96\x74\x37\xC0\xBC\x12\xB4\xB1\x84\x9A\xC5\xE5\xA0\x0E\x39\x8B\x8E\x11\x11\x36\xE6\x21\x35\x3C\x34\x39\x17\x27\x76\x29\xB2\x29\x3B\x9B\x98\x1E\xC1\x6C\x03\xFE\x03\x16\x25\xF2\xF2\x1E\x2F\x60\xE2\xE8\xF9\xE5\x30\xE5\xB1\xCA\xCC\xDA\x9C\x83\x13\x29\xD9\x32\x63\x74\x2C\x8E\x37\xE9\xEE\x39\xED\xDD\x74\x4A\x7A\x1C\xE3\x66\x77\x79\x7A\x6F\x21\x74\x80\x0F\x62\x2B\x13\x83\x78\x30\x30\x07\x0D\x10\x43\x03\x18\x79\x83\x0F\xAC\x70\x7E\x7B\x1A\x86\x73\x1D\x50\xBD\x46\x60\x5D\x53\x23\x8C\x19\x1A\x55\x9E\x69\x80\x03\x88\x03\x63\x8D\x13\x89\x63\x2A\x8E\x65\x65\x50\x74\x84\x41\x1A\x62\x22\x1C\x66\x8C\x91\x76\x83\x42\x8D\x53\x23\x12\x1D\x82\x6C\x3C\x81\x64\x62\x03\x48\x64\x15\x18\x62\x06\x96\x63\x01\x5B\x50\x91\x5F\x5C\x74\x02\x1F\x18\x1E\x4E\x9B\x59\x02\x96\x02\x54\x1F\x19\x82\x56\x5B\x70\x1D\x96\x35\x96\x61\x43\x15\x24\x64\x1F\x1B\x1D\x64\x8D\x5C\x03\x56\x6F\x81\x47\x11\x18\x63\x07\x19\x4F\x6F\x6E\x26\x19\x1B\x19\x46\xA9\x60\x54\x92\x02\x48\x63\x10\x1C\x63\x9B\x5C\x53\x9F\x5B\x5D\x5D\x52\xA2\x83\xB0\x1F\x6D\x1B\x70\x32\x19\x43\x1C\x65\x24\x14\x07\x56\x97\xAA\x5F\x45\x48\x23\x6E\x13\x53\x42\x0D\x65\x1E\x0B\x18\x84\x80\xA1\x40\x8B\x02\x63\x8F\x5D\x19\x8A\x43\x41\x6B\xA6\x8F\x6E\x16\xA8\x57\x89\x82\x4E\x13\x60\x6D\x21\x1C\x18\x55\x63\x2C\x15\x1F\x4E\xAD\x50\x51\xB0\x00\x2E\x2E\x0F\x09\xB1\xC7\x86\xB3\x9F\x18\x55\x09\x1E\x08\x1A\xB3\x91\x0B\x56\xB4\x01\x48\x46\x19\x59\xF9\x7B\x7C\xAE\xAF\x3B\x79\x0B\x56\xB7\x01\x48\xB2\xBA\xB1\xD6\x98\xBA\x7D\x4B\x2F\x14\x19\x1C\x4E\x36\x73\x12\x91\x1A\x70\x1B\x6A\x4B\x0B\x47\x35\x19\x19\xB9\xF8\x67\xBE\x18\x14\xBE\xA7\x14\x4B\xB3\xB3\xB1\x9E\x9A\x40\x53\x20\xAE\x42\x11\x09\xBB\x42\x1E\x8F\x02\x40\x96\xC1\x19\x86\x4B\x0B\x56\xC4\x01\x50\x12\xA6\x60\x90\xA9\x15\x59\x61\x5B\x7E\x94\x8B\x5D\x20\xF9\x5B\x5D\x5F\xAB\x81\x63\x60\x19\x43\x21\xCD\x50\x5E\x00\x14\x40\x4B\x1C\x9D\xBA\x0A\x0C\x15\x68\x25\x1A\x44\x06\x5B\x78\xC1\x42\xCB\x6B\x30\xC4\x4A\xCC\x98\x5A\x6D\x4A\x5C\x90\xBB\x51\x72\x06\x5B\x48\xDB\xCF\xA6\x3E\xEE\x12\xCF\x10\x35\xE3\x1B\x1A\x95\x78\x04\xD0\x1E\xD1\x65\x1D\x0B\x56\xD6\x4B\xEF\xC1\xCE\x0E\x52\x13\x4C\x49\xD0\x9B\x6E\x16\xD4\xD6\x1F\x1B\x59\xD9\xD7\x3D\xDF\xD5\x6D\x69\x74\x0C\x1C\x91\x6C\x67\xD5\x45\x19\x08\x5B\x78\xDC\xD9\xD3\x32\xEF\xD7\x6E\x5C\x74\x54\xD5\xDD\x70\x20\x1B\x55\xE3\xDE\x6E\xEC\x56\x6E\x69\x56\x3E\x47\x1D\x4A\x83\xE5\x1A\xD1\x53\x93\xFC\xCC\xDE\x0E\x8F\xBA\x82\xE2\xCD\x57\xF6\xD4\x0B\x56\x9F\xC8\xEF\xCE\x0E\x45\x38\x10\x1D\x18\x3A\x64\x47\x1A\x4D\x82\x72\x15\x47\x60\x91\xC7\xED\x52\xEC\x96\xDF\xD9\x11\x70\x6D\x02\x14\x9F\x1A\x67\x03\x95\x1E\x4E\x91\xE6\x0B\x56\xF0\xA2\xCE\xDA\x0E\x89\x56\x4D\xE5\x1B\x1D\xBF\x7E\xAD\x1A\x1B\x48\x25\x13\x44\xE4\x9C\xD8\xD3\xE7\x40\x95\xCC\xDF\xD4\xF2\x74\x0A\xF7\x1D\xE4\xD7\xC2\x49\xF6\xD7\xA3\xC0\xE1\xDF\x89\x74\x20\xF9\xD2\x40\xB5\xE5\xF2\xA5\x19\xDE\xD1\xEC\x07\x56\xF3\xC5\xFD\xDE\xDD\x50\x29\x12\xD4\xF5\x45\xF6\xD1\xF9\x40\xE3\x6D\x6F\xE8\x47\x0E\x76\x38\x1B\x75\x7F\xC1\x37\x03\x56\x0A\x86\x7F\xE3\x73\x6C\x0D\x91\x7C\x69\x53\x12\x8D\x7C\xE3\x53\x08\x0D\x83\x34\x0F\x68\x74\x0B\x09\x32\x39\x09\x72\x22\x1B\x29\x08\x8D\x82\x0A\x1C\x7A\x46\x0D\x53\x17\x69\x1A\x74\x01\x09\xFF\x2B\x49\x72\x6B\x6D\x29\x0C\x95\x82\x67\x12\xB9\x45\x25\x64\x6D\x27\x1C\x11\x7D\x05\xAD\x3D\x84",
				"\x84\x26\x8D\x27\x9C\x6C\x05\x2C\x39\x39\x0C\x08\x43\x50\x0E\x9A\x77\x0E\x0D\x1C\xA6\x6D\x7A\x76\x67\x07\x56\x52\x80\x89\x1B\xAE\x0B\x88\x45\x99\x0F\xED\x36\x6C\x7A\x18\xAA\x6C\x7D\xDA\x63\x76\x0D\xA7\x88\x8B\x94\x1F\x80\x8A\xC1\x29\x07\x56\x6A\x85\x8A\x32\x85\x85\x74\xA7\x7A\x26\x13\x91\x7A\x6F\xA6\x35\x8C\x86\x60\x61\x8B\x15\x85\x80\x22\x37\x35\x09\x82\x5B\x60\x8C\xF9\x46\x73\x8C\x21\x85\x84\x8E\x79\x62\x92\x00\x84\x9A\x8F\x24\x02\x36\x0D\x72\x62\x0C\xDD\x68\x85\x0D\x11\x1B\x2E\x92\x6D\x8D\x26\xDC\x72\x07\x0D\x32\x33\x8C\x92\x2B\x1B\x29\x28\x99\x90\x93\x8F\x1D\x90\x89\x6F\x0A\x8B\x9A\x4D\x89\x33\xB2\x51\x70\x05\x5B\x30\x94\x29\x9B\x9D\x93\x14\xB2\x35\x4A\xAD\x83\x68\x25\x98\x9D\x28\x5E\xB8\x8D\x90\x28\x99\x54\x11\x9F\x98\x81\x32\x23\x95\x29\xC7\x9F\x96\x1E\xD3\x02\x98\x36\x28\x92\x95\x4C\x84\x99\xE7\x41\x34\x79\xA6\x15\x9A\x99\x56\x8C\x9A\xAE\x3B\x5F\x91\x48\x93\x95\x98\xB2\x8D\x29\x38\xD8\x97\x84\x6D\x84\x8E\x96\x2B\x92\x98\xD6\x54\x8D\x28\x76\xA4\x9B\x68\x5F\x4C\x0D\x98\x70\x51\x7B\xE9\x72\x09\x72\x25\x1B\x29\x3E\xA4\x9E\x4A\x30\x74\x0A\x4C\x7D\x9F\x8C\x25\xA3\x8D\x28\x82\xAF\x9E\x6C\xF1\x93\x9C\x1C\x3B\x84\x93\xD0\x4D\x2E\xA1\x78\x96\x4A\xD0\x6D\x01\x49\x21\x43\x99\x97\x4F\x85\x99\x0C\x83\x23\x87\x30\x86\x78\x09\x15\xA4\x48\x12\x28\x74\x0D\xB2\x11\x71\x03\x5B\x28\xA5\x03\xA0\xA2\x48\x22\x6A\x78\x2A\x01\x97\x70\x12\x54\x0E\x75\xFD\x77\x9B\xA0\x22\x8A\x93\xE5\x4C\x05\x0C\x33\x34\x09\xA4\x17\xA2\x69\x46\xD3\x99\x6B\x40\x8D\x86\x62\x6F\x1D\x77\xA4\x3A\x77\xA7\x62\xBC\x93\x20\xBE\x9E\xA1\x0D\xFB\x48\x0D\x9F\xAD\xA1\xA8\xAE\x94\x91\xFD\x4D\x2B\xAA\x3C\x80\xAC\x0D\xF1\x74\x91\x1D\x83\x27\x8E\xA9\xB3\x61\x95\x5F\xAA\x9E\xDD\x1B\xA2\x21\x8E\x83\x92\x66\x56\x1F\xAB\x48\xBA\x26\xA4\x4A\xAA\xA5\x29\x77\xB8\x8D\x5C\xD7\xA0\x82\x4A\xBF\x78\x00\x31\xBF\x6A\x15\x34\xAB\x76\xFE\x44\xAB\xAC\x02\x25\xAF\x5B\xB3\x6C\x49\x7A\x8A\x62\xAD\xC1\x3B\x03\x56\x12\xBD\x81\x25\x62\x0E\xB1\x86\xB8\x69\x05\x5B\x2B\xB3\xCA\x8C\x48\x84\x39\xAD\x81\x13\x54\x3A\x3E\xD9\x2E\x0D\x0C\xE6\x67\x80\x51\xCF\xA2\x21\xA8\x8B\xB3\x0A\xAA\x65\x2E\x69\xA8\xB4\x4A\xD5\xA8\x6F\x8B\xA2\xB9\x25\x1D\x4C\xA0\xB3\x32\x32\x73\x20\xB4\x66\xAD\x40\xDB\x89\x76\x8C\x91\x76\x03\x5B\x28\xB9\x6E\xFE\x88\x9C\xCD\x19\xB6\x6E\xFB\x8D\x29\x74\xF8\x8D\x9E\x79\xBF\x98\x5E\xCD\xAD\x87\x90\x5F\x72\xAD\x44\x95\xB1\xA1\xD7\xA6\x82\x71\x94\x9C\x8D\xA6\x25\xBC\xBA\xE0\xA2\x4F\x33\xCB\x8C\x94\xA1\x9E\x88\x2A\x6E\xAB\x92\xCC\x55\xBA\xA1\x80\x81\x7B\x78\x3A\xBF\x68\x7D\xE1\xBD\xB9\xDE\xAC\xB1\x20\xAE\xB2\xBF\x59\xFE\x92\x4D\xE6\x81\x3D\xA9\x44\xB4\xB1\x53\x0A\xC8\xBC\x02\xF0\x5F\xBD\x32\x99\xBF\xE8\x43\x05\x0E\xAB\x2F\x0D\x0D\x78\x5F\x9D\x82\x27\xA9\xA4\x09\xC6\x7D\xBA\xFA\x30\x0D\xE4\x3A\x6D\x28\x12\xF8\x88\x4D\x3A\x2F\x84\x25\x86\x30",
				};

			class DarkSkinResourceLoaderPlugin : public Object, public IGuiPlugin
			{
			public:

				GUI_PLUGIN_NAME(GacGen_DarkSkinResourceLoader)
				{
					GUI_PLUGIN_DEPEND(GacUI_Res_Resource);
					GUI_PLUGIN_DEPEND(GacUI_Res_TypeResolvers);
#ifndef VCZH_DEBUG_NO_REFLECTION
					GUI_PLUGIN_DEPEND(GacUI_Instance_Reflection);
					GUI_PLUGIN_DEPEND(GacUI_Compiler_WorkflowTypeResolvers);
#endif
				}

				void Load()override
				{
					List<GuiResourceError> errors;
					MemoryStream resourceStream;
					DarkSkinResourceReader::ReadToStream(resourceStream);
					resourceStream.SeekFromBegin(0);
					GetResourceManager()->LoadResourceOrPending(resourceStream, GuiResourceUsage::InstanceClass);
				}

				void Unload()override
				{
				}
			};
			GUI_REGISTER_PLUGIN(DarkSkinResourceLoaderPlugin)
		}
	}
}
