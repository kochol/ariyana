using System;
using System.Collections;

namespace ari.gfx.Material
{
	class Technique
	{
		public String ps;
		public String ps_file;
		public String vs;
		public String vs_file;

		public List<Constant> constants;
		public int constants_used_bytes;
		public int constants_size_bytes;

		public String name;

		public int permutation_id;
		public int permutation_option_mask;
		public List<Permutation> permutations;

		public List<TextureSample> texture_sampler_bindings;

		public List<CBuffer> cbuffers;

		public List<VsInput> vs_inputs;

		public List<InstanceInput> instance_inputs;

		public List<VsOutput> vs_outputs;
	}
}
