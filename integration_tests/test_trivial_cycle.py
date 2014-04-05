#! /usr/bin/python

from nose.tools import assert_equal, assert_almost_equal, assert_true
from numpy.testing import assert_array_equal
import os
import tables
import numpy as np
from tools import check_cmd
from helper import table_exist, find_ids, exit_times

def create_sim_input(ref_input, k_factor_in, k_factor_out):
    """ Creates xml input file from a reference xml input file.
    Changes k_factor_in_ and k_factor_out_.

    Returns: the path to the created file
    """
    # File to be creted
    fw_path = ref_input.split(".xml")[0] + "_" + str(k_factor_in) + \
              "_" + str(k_factor_out) + ".xml"
    fw = open(fw_path, "w")
    fr = open(ref_input, "r")
    for f in fr:
        if f.count("k_factor_in_"):
            f = f.split("<")[0] + "<k_factor_in_>" + str(k_factor_in) + \
                "</k_factor_in_>\n"
        elif f.count("k_factor_out_"):
            f = f.split("<")[0] + "<k_factor_out_>" + str(k_factor_out) + \
                "</k_factor_out_>\n"

        fw.write(f)

    # Closing open files
    fr.close()
    fw.close()

    return fw_path


""" Tests """
def test_source_to_sink():
    """ Tests simulations with a facilty that has a conversion factor.
    In future, may eliminate checks if needed tables exist, and rely on errors.
    In future, may eliminate tests for the existance and uniqueness of the
    facilities. In addition, may eliminate other non-integration testing relevant
    code and tests.
    """
    # Cyclus simulation input for source_to_sink
    ref_input = "./Inputs/trivial_cycle.xml"
    # Conversion factors for the three simulations
    k_factors = [0.95, 1, 2]

    for k_factor in k_factors:
        sim_input = create_sim_input(ref_input, k_factor, k_factor)

        holdsrtn = [1]  # needed because nose does not send() to test generator
        cmd = ["cyclus", "-o", "./output_temp.h5", "--input-file", sim_input]
        yield check_cmd, cmd, '.', holdsrtn
        rtn = holdsrtn[0]
        if rtn != 0:
            return  # don't execute further commands

        output = tables.open_file("./output_temp.h5", mode = "r")
        # tables of interest
        paths = ["/AgentEntry", "/Resources", "/Transactions",
                "/Info"]
        # Check if these tables exist
        yield assert_true, table_exist(output, paths)
        if not table_exist(output, paths):
            output.close()
            os.remove("./output_temp.h5")
            # This is a starter sqlite db created implicitly
            os.remove("./output_temp.sqlite")
            return  # don't execute further commands

        # Get specific tables and columns
        agent_entry = output.get_node("/AgentEntry")[:]
        info = output.get_node("/Info")[:]
        resources = output.get_node("/Resources")[:]
        transactions = output.get_node("/Transactions")[:]

        # Find agent ids
        agent_ids = agent_entry["AgentId"]
        agent_impl = agent_entry["Implementation"]
        duration = info["Duration"][0]

        facility_id = find_ids("KFacility", agent_impl, agent_ids)
        # Test for only one KFacility
        yield assert_equal, len(facility_id), 1

        sender_ids = transactions["SenderId"]
        receiver_ids = transactions["ReceiverId"]
        expected_sender_array = np.empty(sender_ids.size)
        expected_sender_array.fill(facility_id[0])
        expected_receiver_array = np.empty(receiver_ids.size)
        expected_receiver_array.fill(facility_id[0])
        yield assert_array_equal, sender_ids, expected_sender_array
        yield assert_array_equal, receiver_ids, expected_receiver_array

        # Transaction ids must be equal range from 1 to the number of rows
        expected_trans_ids = np.arange(0, sender_ids.size, 1)
        yield assert_array_equal, transactions["TransactionId"], expected_trans_ids

        # Track transacted resources
        resource_ids = resources["ResourceId"]
        quantities = resources["Quantity"]

        # Almost equal cases due to floating point k_factors
        i = 0
        initial_capacity = quantities[0]
        for q in quantities:
            yield assert_almost_equal, q, initial_capacity * k_factor ** i
            i += 1

        output.close()
        os.remove("./output_temp.h5")
        os.remove(sim_input)
        # This is a starter sqlite db created implicitly
        os.remove("./output_temp.sqlite")