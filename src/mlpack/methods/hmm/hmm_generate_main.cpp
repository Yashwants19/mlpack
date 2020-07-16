/**
 * @file methods/hmm/hmm_generate_main.cpp
 * @author Ryan Curtin
 * @author Michael Fox
 *
 * Compute the most probably hidden state sequence of a given observation
 * sequence for a given HMM.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/io.hpp>
#include <mlpack/core/util/mlpack_main.hpp>

#include "hmm.hpp"
#include "hmm_model.hpp"

#include <mlpack/methods/gmm/gmm.hpp>
#include <mlpack/methods/gmm/diagonal_gmm.hpp>

using namespace mlpack;
using namespace mlpack::hmm;
using namespace mlpack::distribution;
using namespace mlpack::util;
using namespace mlpack::gmm;
using namespace mlpack::math;
using namespace arma;
using namespace std;

PROGRAM_INFO("Hidden Markov Model (HMM) Sequence Generator",
    // Short description.
    "A utility to generate random sequences from a pre-trained Hidden Markov "
    "Model (HMM).  The length of the desired sequence can be specified, and a "
    "random sequence of observations is returned.",
    // Long description.
    "This utility takes an already-trained HMM, specified as the " +
    PRINT_PARAM_STRING("model") + " parameter, and generates a random "
    "observation sequence and hidden state sequence based on its parameters. "
    "The observation sequence may be saved with the " +
    PRINT_PARAM_STRING("output") + " output parameter, and the internal state "
    " sequence may be saved with the " + PRINT_PARAM_STRING("state") + " output"
    " parameter."
    "\n\n"
    "The state to start the sequence in may be specified with the " +
    PRINT_PARAM_STRING("start_state") + " parameter."
    "\n\n",
    // Example.
    "For example, to generate a sequence of length 150 from the HMM " +
    PRINT_MODEL("hmm") + " and save the observation sequence to " +
    PRINT_DATASET("observations") + " and the hidden state sequence to " +
    PRINT_DATASET("states") + ", the following command may be used: "
    "\n\n" +
    PRINT_CALL("hmm_generate", "model", "hmm", "length", 150, "output",
        "observations", "state", "states"),
    SEE_ALSO("@hmm_train", "#hmm_train"),
    SEE_ALSO("@hmm_loglik", "#hmm_loglik"),
    SEE_ALSO("@hmm_viterbi", "#hmm_viterbi"),
    SEE_ALSO("Hidden Mixture Models on Wikipedia",
        "https://en.wikipedia.org/wiki/Hidden_Markov_model"),
    SEE_ALSO("mlpack::hmm::HMM class documentation",
        "@doxygen/classmlpack_1_1hmm_1_1HMM.html"));

PARAM_MODEL_IN_REQ(HMMModel, "model", "Trained HMM to generate sequences with.",
    "m");
PARAM_INT_IN_REQ("length", "Length of sequence to generate.", "l");

PARAM_INT_IN("start_state", "Starting state of sequence.", "t", 0);
PARAM_MATRIX_OUT("output", "Matrix to save observation sequence to.", "o");
PARAM_UMATRIX_OUT("state", "Matrix to save hidden state sequence to.", "S");
PARAM_INT_IN("seed", "Random seed.  If 0, 'std::time(NULL)' is used.", "s", 0);

// Because we don't know what the type of our HMM is, we need to write a
// function which can take arbitrary HMM types.
struct Generate
{
  template<typename HMMType>
  static void Apply(HMMType& hmm, void* /* extraInfo */)
  {
    mat observations;
    Row<size_t> sequence;

    RequireParamValue<int>("start_state", [](int x) { return x >= 0; }, true,
        "Invalid start state");
    RequireParamValue<int>("length", [](int x) { return x >= 0; }, true,
        "Length must be >= 0");

    // Load the parameters.
    const size_t startState = (size_t) IO::GetParam<int>("start_state");
    const size_t length = (size_t) IO::GetParam<int>("length");

    Log::Info << "Generating sequence of length " << length << "..." << endl;
    if (startState >= hmm.Transition().n_rows)
    {
      Log::Fatal << "Invalid start state (" << startState << "); must be "
          << "between 0 and number of states (" << hmm.Transition().n_rows
          << ")!" << endl;
    }

    hmm.Generate(length, observations, sequence, startState);

    // Now save the output.
    if (IO::HasParam("output"))
      IO::GetParam<mat>("output") = std::move(observations);

    // Do we want to save the hidden sequence?
    if (IO::HasParam("state"))
      IO::GetParam<Mat<size_t>>("state") = std::move(sequence);
  }
};

static void mlpackMain()
{
  RequireAtLeastOnePassed({ "output", "state" }, false, "no output will be "
      "saved");

  // Set random seed.
  if (IO::GetParam<int>("seed") != 0)
    RandomSeed((size_t) IO::GetParam<int>("seed"));
  else
    RandomSeed((size_t) time(NULL));

  // Load model, and perform the generation.
  HMMModel* hmm;
  hmm = std::move(IO::GetParam<HMMModel*>("model"));
  hmm->PerformAction<Generate, void>(NULL); // No extra data required.
}
