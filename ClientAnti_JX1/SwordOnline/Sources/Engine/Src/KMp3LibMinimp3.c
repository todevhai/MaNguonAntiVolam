/* mp3lib API (Include/mp3lib.h) implemented on minimp3 (Src/minimp3/minimp3.h, CC0).
 *
 * KMp3Music linked the Kingsoft mp3lib.lib statically; that library shipped without source,
 * so the background-music decoder is rebuilt here from source. Same global-decoder model as
 * the old library: mp3_decode_head probes the stream, mp3_decode_init resets the decoder,
 * mp3_decode_frame decodes one frame into 16-bit interleaved PCM.
 *
 * The old API had no input length; minimp3 must know how many bytes are readable to validate
 * frame sync, so head/frame take the byte count the caller already tracks (mp3_bufbytes). */
#define MINIMP3_IMPLEMENTATION
#define MINIMP3_NO_SIMD
#include "minimp3/minimp3.h"
#include "mp3lib.h"
#include <string.h>

static mp3dec_t	s_Dec;
static int		s_nChannels = 2;
static int		s_nSampleRate = 44100;
static int		s_nFrameBytes = 0;

/* Returns the size of the first MPEG audio frame (0 = no frame found). ID3 tags and other
 * leading data are skipped by minimp3; the skipped bytes are consumed later by
 * mp3_decode_frame (in_bytes > 0, out_bytes == 0). */
int mp3_decode_head(unsigned char *buf, int buf_bytes, MPEG_HEAD *head)
{
	mp3dec_t dec;
	mp3dec_frame_info_t info;
	static mp3d_sample_t pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
	int nOffset = 0;
	int nLoop;

	memset(head, 0, sizeof(*head));
	mp3dec_init(&dec);
	/* The first layer III frames may decode to 0 samples (bit reservoir); walk a few. */
	for (nLoop = 0; nLoop < 16 && nOffset < buf_bytes; nLoop++)
	{
		memset(&info, 0, sizeof(info));
		mp3dec_decode_frame(&dec, buf + nOffset, buf_bytes - nOffset, pcm, &info);
		if (info.frame_bytes <= 0)
			return 0;
		if (info.channels > 0 && info.hz > 0)
		{
			s_nChannels = info.channels;
			s_nSampleRate = info.hz;
			s_nFrameBytes = info.frame_bytes - info.frame_offset;
			head->sync = 0xFFF;
			head->layer = 4 - info.layer;		/* 1=LayerIII 2=LayerII 3=LayerI */
			head->mode = info.channels == 1 ? 3 : 0;
			return s_nFrameBytes;
		}
		nOffset += info.frame_bytes;
	}
	return 0;
}

int mp3_decode_init(MPEG_HEAD *head, int framebytes_arg, int reduction_code,
					int transform_code, int convert_code, int freq_limit)
{
	(void)head; (void)framebytes_arg; (void)reduction_code;
	(void)transform_code; (void)convert_code; (void)freq_limit;
	mp3dec_init(&s_Dec);
	return 1;
}

void mp3_decode_info(DEC_INFO *info)
{
	info->channels = s_nChannels;
	info->outvalues = 1152 * s_nChannels;
	info->samprate = s_nSampleRate;
	info->bits = 16;
	info->framebytes = s_nFrameBytes;
}

IN_OUT mp3_decode_frame(unsigned char *mp3, int mp3_bytes, unsigned char *pcm)
{
	IN_OUT res = {0, 0};
	mp3dec_frame_info_t info;
	int nSamples;

	memset(&info, 0, sizeof(info));
	nSamples = mp3dec_decode_frame(&s_Dec, mp3, mp3_bytes, (mp3d_sample_t *)pcm, &info);
	res.in_bytes = info.frame_bytes;
	res.out_bytes = nSamples * info.channels * (int)sizeof(mp3d_sample_t);
	return res;
}
