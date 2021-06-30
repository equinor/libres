from pathlib import Path
import pytest
import shutil

from res.enkf import ResConfig, EnKFMain
from tests.conftest import source_root
import os
from tests import ResTest
from res.test import ErtTestContext
from pytest import MonkeyPatch
from ecl.summary import EclSum
from res.enkf.export import SummaryObservationCollector


class SummaryObservationCollectorTest(ResTest):
    def setUp(self):
        self.monkeypatch = MonkeyPatch()
        self.monkeypatch.setenv(
            "TZ", "CET"
        )  # The ert_statoil case was generated in CET
        self.config = self.createTestPath("local/snake_oil/snake_oil.ert")

    def tearDown(self):
        self.monkeypatch.undo()

    def test_summary_observation_collector(self):

        with ErtTestContext(
            "python/enkf/export/summary_observation_collector", self.config
        ) as context:

            ert = context.getErt()

            self.assertTrue(
                SummaryObservationCollector.summaryKeyHasObservations(ert, "FOPR")
            )
            self.assertFalse(
                SummaryObservationCollector.summaryKeyHasObservations(ert, "FOPT")
            )

            keys = SummaryObservationCollector.getAllObservationKeys(ert)
            self.assertTrue("FOPR" in keys)
            self.assertTrue("WOPR:OP1" in keys)
            self.assertFalse("WOPR:OP2" in keys)

            data = SummaryObservationCollector.loadObservationData(ert, "default_0")

            self.assertFloatEqual(data["FOPR"]["2010-01-10"], 0.001696887)
            self.assertFloatEqual(data["STD_FOPR"]["2010-01-10"], 0.1)

            self.assertFloatEqual(data["WOPR:OP1"]["2010-03-31"], 0.1)
            self.assertFloatEqual(data["STD_WOPR:OP1"]["2010-03-31"], 0.05)

            with self.assertRaises(KeyError):
                fgir = data["FGIR"]

            data = SummaryObservationCollector.loadObservationData(
                ert, "default_0", ["WOPR:OP1"]
            )

            self.assertFloatEqual(data["WOPR:OP1"]["2010-03-31"], 0.1)
            self.assertFloatEqual(data["STD_WOPR:OP1"]["2010-03-31"], 0.05)

            with self.assertRaises(KeyError):
                data["FOPR"]


test_data_root = Path(source_root()) / "test-data" / "local"


@pytest.fixture()
def setup_snake_oil(tmpdir):
    with tmpdir.as_cwd():
        test_data_dir = os.path.join(test_data_root, "snake_oil")

        shutil.copytree(test_data_dir, "test_data")
        os.chdir("test_data")

        yield ResConfig("snake_oil.ert")


def test_summary_obs_last_entry(setup_snake_oil):

    obs_file = Path.cwd() / "observations" / "observations.txt"
    with obs_file.open(mode="a") as fin:
        fin.write(
            """
            \nSUMMARY_OBSERVATION LAST_DATE
{
    VALUE   = 0.01961757428944111;
    ERROR   = 0.1;
    DATE    = 23/06/2015;
    KEY     = FOPR;
};
            """
        )

    ert = EnKFMain(setup_snake_oil)

    summary = EclSum("refcase/SNAKE_OIL_FIELD.UNSMRY")
    data = SummaryObservationCollector.loadObservationData(ert, "default_0")
    assert (
        data["FOPR"].values.tolist()
        == summary.numpy_vector("FOPRH", report_only=True).tolist()
    )
