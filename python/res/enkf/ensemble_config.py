#  Copyright (C) 2012  Equinor ASA, Norway.
#
#  The file 'ensemble_config.py' is part of ERT - Ensemble based Reservoir Tool.
#
#  ERT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ERT is distributed in the hope that it will be useful, but WITHOUT ANY
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.
#
#  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
#  for more details.
from cwrap import BaseCClass
from ecl.util.util import StringList
from ecl.grid import EclGrid
from ecl.summary import EclSum
from res import ResPrototype
from res.enkf import SummaryKeyMatcher, ConfigKeys
from res.config import ConfigContent
from res.enkf.config import EnkfConfigNode, CustomKWConfig
from res.enkf.enums import EnkfVarType, ErtImplType, LoadFailTypeEnum
import os


def _get_abs_path(file):
    if file is not None:
        file = os.path.realpath(file)
    return file


def _get_filename(file):
    if file is not None:
        file = os.path.basename(file)
    return file

class EnsembleConfig(BaseCClass):
    TYPE_NAME = "ens_config"
    _alloc = ResPrototype("void* ensemble_config_alloc(config_content, ecl_grid, ecl_sum)", bind=False)
    _alloc_load = ResPrototype("void* ensemble_config_alloc_load(char* , ecl_grid, ecl_sum)", bind=False)
    _alloc_full = ResPrototype("void* ensemble_config_alloc_full(char*)", bind=False)
    _free = ResPrototype("void ensemble_config_free( ens_config )")
    _has_key = ResPrototype("bool ensemble_config_has_key( ens_config , char* )")
    _size = ResPrototype("int ensemble_config_get_size( ens_config)")
    _get_node = ResPrototype("enkf_config_node_ref ensemble_config_get_node( ens_config , char*)")
    _alloc_keylist = ResPrototype("stringlist_obj ensemble_config_alloc_keylist( ens_config )")
    _add_summary = ResPrototype("enkf_config_node_ref ensemble_config_add_summary( ens_config, char*, int)")
    _add_gen_kw = ResPrototype("enkf_config_node_ref ensemble_config_add_gen_kw( ens_config, char*)")
    _add_field = ResPrototype("enkf_config_node_ref ensemble_config_add_field( ens_config, char*, ecl_grid)")
    _alloc_keylist_from_var_type = ResPrototype("stringlist_obj ensemble_config_alloc_keylist_from_var_type(ens_config, enkf_var_type_enum)")
    _alloc_keylist_from_impl_type = ResPrototype("stringlist_obj ensemble_config_alloc_keylist_from_impl_type(ens_config, ert_impl_type_enum)")
    _add_node = ResPrototype("void ensemble_config_add_node( ens_config , enkf_config_node )")
    _summary_key_matcher = ResPrototype("summary_key_matcher_ref ensemble_config_get_summary_key_matcher(ens_config)")
    _add_defined_custom_kw = ResPrototype("enkf_config_node_ref ensemble_config_add_defined_custom_kw(ens_config, char*, integer_hash)")
    _get_trans_table = ResPrototype("void* ensemble_config_get_trans_table(ens_config)")
    _add_summary_full = ResPrototype("void ensemble_config_init_SUMMARY_full(ens_config, char*, ecl_sum)")


    def __init__(self, config_content=None, grid=None, refcase=None, config_dict=None):
        if config_content is not None and config_dict is not None:
            raise ValueError("Attempting to create EnsembleConfig object with multiple config objects")
        
        c_ptr = None
        if config_dict is not None:
            c_ptr = self._alloc_full(config_dict[ConfigKeys.GEN_KW_TAG_FORMAT])
            if c_ptr is None:
                raise ValueError("Failed to construct EnsembleConfig instance from dict")

            super(EnsembleConfig, self).__init__(c_ptr)

            gen_param_list = config_dict.get(ConfigKeys.GEN_PARAM, [])
            for a in gen_param_list:
                gen_param_node = EnkfConfigNode.create_gen_param(a[ConfigKeys.NAME],
                                                                 a[ConfigKeys.FORWARD_INIT],
                                                                 a[ConfigKeys.INPUT_FORMAT],
                                                                 a[ConfigKeys.OUTPUT_FORMAT],
                                                                 a[ConfigKeys.INIT_FILES],
                                                                 a[ConfigKeys.ECL_FILE],
                                                                 a[ConfigKeys.MIN_STD],
                                                                 a[ConfigKeys.TEMPLATE],
                                                                 a[ConfigKeys.KEY_KEY])
                self.addNode(gen_param_node)

            gen_data_list = config_dict.get(ConfigKeys.GEN_DATA, [])
            for a in gen_data_list:
                gen_data_node = EnkfConfigNode.create_gen_data_full(a[ConfigKeys.NAME],
                                                                    a[ConfigKeys.RESULT_FILE],
                                                                    a[ConfigKeys.INPUT_FORMAT],
                                                                    a[ConfigKeys.REPORT_STEPS],
                                                                    a[ConfigKeys.ECL_FILE],
                                                                    a[ConfigKeys.INIT_FILES],
                                                                    a[ConfigKeys.TEMPLATE],
                                                                    a[ConfigKeys.KEY_KEY])
                self.addNode(gen_data_node)

            custom_kw_list = config_dict.get(ConfigKeys.CUSTOM_KW, [])
            for a in custom_kw_list:
                custom_kw_node = EnkfConfigNode.create_custom_kw(a[ConfigKeys.NAME],
                                                                 a[ConfigKeys.RESULT_FILE],
                                                                 a[ConfigKeys.OUT_FILE])
                self.addNode(custom_kw_node)

            gen_kw_list = config_dict.get(ConfigKeys.GEN_KW, [])
            for a in gen_kw_list:
                gen_kw_node = EnkfConfigNode.create_gen_kw(a[ConfigKeys.NAME],
                                                           _get_abs_path(a[ConfigKeys.TEMPLATE]),
                                                           a[ConfigKeys.OUT_FILE],
                                                           _get_abs_path(a[ConfigKeys.PARAMETER_FILE]),
                                                           a[ConfigKeys.FORWARD_INIT],
                                                           a[ConfigKeys.MIN_STD],
                                                           a[ConfigKeys.INIT_FILES],
                                                           config_dict[ConfigKeys.GEN_KW_TAG_FORMAT])
                self.addNode(gen_kw_node)

            surface_list = config_dict.get(ConfigKeys.SURFACE_KEY, [])
            for a in surface_list:
                surface_node = EnkfConfigNode.create_surface(a[ConfigKeys.NAME],
                                                             a[ConfigKeys.INIT_FILES],
                                                             a[ConfigKeys.OUT_FILE],
                                                             a[ConfigKeys.BASE_SURFACE_KEY],
                                                             a[ConfigKeys.MIN_STD],
                                                             a[ConfigKeys.FORWARD_INIT])
                self.addNode(surface_node)

            summary_list = config_dict.get(ConfigKeys.SUMMARY, [])
            for a in summary_list:
                self.add_summary_full(a[ConfigKeys.NAME], refcase)

            field_list = config_dict.get(ConfigKeys.FIELD_KEY, [])
            for a in field_list:
                field_node = EnkfConfigNode.create_field(a[ConfigKeys.NAME],
                                                         a[ConfigKeys.VAR_TYPE],
                                                         grid,
                                                         self._get_trans_table(),
                                                         a[ConfigKeys.OUT_FILE],
                                                         a[ConfigKeys.ENKF_INFILE],
                                                         a[ConfigKeys.FORWARD_INIT],
                                                         a[ConfigKeys.INIT_TRANSFORM],
                                                         a[ConfigKeys.OUTPUT_TRANSFORM],
                                                         a[ConfigKeys.INPUT_TRANSFORM],
                                                         a[ConfigKeys.MIN_STD],
                                                         a[ConfigKeys.MIN_KEY],
                                                         a[ConfigKeys.MAX_KEY],
                                                         a[ConfigKeys.INIT_FILES])
                self.addNode(field_node)

            schedule_file_list = config_dict.get(ConfigKeys.SCHEDULE_PREDICTION_FILE, [])
            for a in schedule_file_list:
                schedule_file_node = EnkfConfigNode.create_gen_kw(ConfigKeys.PRED_KEY,
                                                                  a[ConfigKeys.TEMPLATE],
                                                                  _get_filename(a[ConfigKeys.TEMPLATE]),
                                                                  a[ConfigKeys.PARAMETER_KEY],
                                                                  False,
                                                                  a[ConfigKeys.MIN_STD],
                                                                  a[ConfigKeys.INIT_FILES],
                                                                  config_dict[ConfigKeys.GEN_KW_TAG_FORMAT])
                self.addNode(schedule_file_node)

            container_list = config_dict.get(ConfigKeys.CONTAINER_KEY, [])
            for a in container_list:
                container_node = EnkfConfigNode.create_container(a[ConfigKeys.NAME])
                for child_key in a[ConfigKeys.ARGLIST]:
                    container_node._update_container(self.getNode(child_key))
                self.addNode(container_node)

            return

        c_ptr = self._alloc(config_content, grid, refcase)
        if c_ptr is None:
            raise ValueError("Failed to construct EnsembleConfig instance")
        super(EnsembleConfig , self).__init__(c_ptr)

    def __len__(self):
        return self._size( )

    def __getitem__(self , key):
        """ @rtype: EnkfConfigNode """
        if key in self:
            return self._get_node(key).setParent(self)
        else:
            raise KeyError("The key:%s is not in the ensemble configuration" % key)

    def getNode(self, key):
        return self[key]

    def alloc_keylist(self):
        """ @rtype: StringList """
        return self._alloc_keylist( )

    def add_summary(self, key):
        """ @rtype: EnkfConfigNode """
        return self._add_summary(key, 2).setParent(self)

    def add_summary_full(self, key, refcase):
        """ @rtype: EnkfConfigNode """
        return self._add_summary_full(key, refcase)

    def add_gen_kw(self, key):
        """ @rtype: EnkfConfigNode """
        return self._add_gen_kw(key).setParent(self)


    def addNode(self , config_node):
        assert isinstance(config_node , EnkfConfigNode)
        self._add_node( config_node )
        config_node.convertToCReference( self )


    def add_field(self, key, eclipse_grid):
        """ @rtype: EnkfConfigNode """
        return self._add_field(key, eclipse_grid).setParent(self)

    def getKeylistFromVarType(self, var_mask):
        """ @rtype: StringList """
        assert isinstance(var_mask, EnkfVarType)
        return self._alloc_keylist_from_var_type(var_mask)

    def getKeylistFromImplType(self, ert_impl_type):
        """ @rtype: StringList """
        assert isinstance(ert_impl_type, ErtImplType)
        return self._alloc_keylist_from_impl_type(ert_impl_type)

    def __contains__(self, key):
        return self._has_key( key)

    def getSummaryKeyMatcher(self):
        """ @rtype: SummaryKeyMatcher """
        return self._summary_key_matcher( )

    def free(self):
        self._free( )

    def addDefinedCustomKW(self, group_name, definition):
        """ @rtype: EnkfConfigNode """
        if not group_name in self:
            type_hash = CustomKWConfig.convertDefinition(definition)
            self._add_defined_custom_kw(group_name, type_hash)

        return self[group_name]

    def __eq__(self, other):
        self_param_list = set(self.alloc_keylist())
        other_param_list = set(other.alloc_keylist())
        if self_param_list != other_param_list:
            return False

        for a in self_param_list:
            if a in self and a in other:
                if self.getNode(a) != other.getNode(a):
                    return False
            else:
                return False

        return True

    def __ne__(self, other):
        return not self == other

