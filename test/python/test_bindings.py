import asmc_data_module as dm


def test_strip_back():
    assert dm.stripBack("with whitespace \t\n") == "with whitespace"
