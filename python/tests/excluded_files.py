import os


def get_excluded_files(root):
    excluded_files = [
        "ert/__init__.py",
        "res/analysis/enums/__init__.py",
        "ert/config/__init__.py",
        "res/analysis/enums/analysis_module_load_status_enum.py",
        "ert/enkf/export/__init__.py",
        "res/analysis/enums/analysis_module_options_enum.py",
        "res/analysis/linalg.py",
        "res/config/__init__.py",
        "ert/enkf/enums/__init__.py",
        "res/config/config_path_elm.py",
        "res/config/config_error.py",
        "res/__init__.py",
        "ert/enkf/__init__.py",
        "res/config/unrecognized_enum.py",
        "res/config/schema_item.py",
        "res/config/content_type_enum.py",
        "res/enkf/active_list.py",
        "res/enkf/__init__.py",
        "res/enkf/config/__init__.py",
        "res/config/config_parser.py",
        "res/config/config_settings.py",
        "res/enkf/analysis_iter_config.py",
        "res/enkf/config/field_type_enum.py",
        "res/enkf/config/custom_kw_config.py",
        "res/enkf/config/ext_param_config.py",
        "res/config/config_content.py",
        "res/enkf/config/summary_config.py",
        "res/enkf/config/field_config.py",
        "res/analysis/analysis_module.py",
        "res/enkf/data/__init__.py",
        "res/enkf/custom_kw_config_set.py",
        "res/enkf/config/gen_data_config.py",
        "res/enkf/config/gen_kw_config.py",
        "res/enkf/data/custom_kw.py",
        "res/enkf/data/field.py",
        "res/enkf/analysis_config.py",
        "res/enkf/data/gen_data.py",
        "res/enkf/enkf_defaults.py",
        "res/enkf/data/summary.py",
        "res/enkf/data/enkf_node.py",
        "res/enkf/data/ext_param.py",
        "res/enkf/enkf_linalg.py",
        "res/enkf/data/gen_kw.py",
        "res/enkf/enkf_fs.py",
        "res/enkf/enkf_state.py",
        "res/enkf/config/enkf_config_node.py",
        "res/enkf/enums/__init__.py",
        "res/enkf/ecl_config.py",
        "res/enkf/enums/active_mode_enum.py",
        "res/enkf/enums/enkf_fs_type_enum.py",
        "res/enkf/enkf_simulation_runner.py",
        "res/enkf/enums/enkf_field_file_format_enum.py",
        "res/enkf/enums/enkf_init_modes_enum.py",
        "res/enkf/config_keys.py",
        "res/enkf/enums/enkf_obs_impl_type_enum.py",
        "res/enkf/enums/enkf_run_enum.py",
        "res/enkf/enums/enkf_truncation_type.py",
        "res/enkf/enums/enkf_var_type_enum.py",
        "res/enkf/enums/hook_runtime_enum.py",
        "res/enkf/enkf_fs_manager.py",
        "res/enkf/enums/gen_data_file_type_enum.py",
        "res/enkf/enkf_obs.py",
        "res/enkf/enums/load_fail_type_enum.py",
        "res/enkf/enums/ert_impl_type_enum.py",
        "res/enkf/enums/realization_state_enum.py",
        "res/enkf/export/__init__.py",
        "res/enkf/ert_template.py",
        "res/enkf/es_update.py",
        "res/enkf/export/design_matrix_reader.py",
        "res/enkf/export/arg_loader.py",
        "res/enkf/export/custom_kw_collector.py",
        "res/enkf/export/gen_data_collector.py",
        "res/enkf/ert_templates.py",
        "res/enkf/ert_run_context.py",
        "res/enkf/export/gen_data_observation_collector.py",
        "res/enkf/export/misfit_collector.py",
        "res/enkf/enkf_main.py",
        "res/enkf/export/gen_kw_collector.py",
        "res/enkf/forward_load_context.py",
        "res/enkf/ert_workflow_list.py",
        "res/enkf/export/summary_collector.py",
        "res/enkf/export/summary_observation_collector.py",
        "res/enkf/hook_workflow.py",
        "res/enkf/local_ministep.py",
        "res/enkf/local_obsdata_node.py",
        "res/enkf/ensemble_config.py",
        "res/enkf/local_dataset.py",
        "res/enkf/local_updatestep.py",
        "res/enkf/local_config.py",
        "res/enkf/hook_manager.py",
        "res/enkf/node_id.py",
        "res/enkf/local_obsdata.py",
        "res/enkf/log_config.py",
        "res/enkf/observations/block_data_config.py",
        "res/enkf/key_manager.py",
        "res/enkf/meas_block.py",
        "res/enkf/obs_block.py",
        "res/enkf/meas_data.py",
        "res/enkf/plot/__init__.py",
        "res/enkf/observations/block_observation.py",
        "res/enkf/obs_data.py",
        "res/enkf/plot/data_fetcher.py",
        "res/enkf/observations/gen_observation.py",
        "res/enkf/observations/summary_observation.py",
        "res/enkf/plot/ensemble_gen_data_fetcher.py",
        "res/enkf/observations/obs_vector.py",
        "res/enkf/plot/block_observation_data_fetcher.py",
        "res/enkf/plot/ensemble_block_data_fetcher.py",
        "res/enkf/plot/ensemble_data_fetcher.py",
        "res/enkf/plot_data/__init__.py",
        "res/enkf/plot/ensemble_gen_kw_fetcher.py",
        "res/enkf/model_config.py",
        "res/enkf/plot_data/ensemble_plot_data_vector.py",
        "res/enkf/plot_data/ensemble_plot_gen_data_vector.py",
        "res/enkf/plot_data/ensemble_plot_data.py",
        "res/enkf/plot/observation_data_fetcher.py",
        "res/enkf/plot_data/ensemble_plot_gen_data.py",
        "res/enkf/plot_data/ensemble_plot_gen_kw_vector.py",
        "res/enkf/plot/refcase_data_fetcher.py",
        "res/enkf/plot_data/plot_block_data.py",
        "res/enkf/plot_data/ensemble_plot_gen_kw.py",
        "res/enkf/plot/observation_gen_data_fetcher.py",
        "res/enkf/plot_data/plot_block_vector.py",
        "res/enkf/plot_data/pca_plot_vector.py",
        "res/enkf/plot_data/pca_plot_data.py",
        "res/enkf/plot_data/plot_block_data_loader.py",
        "res/enkf/rng_config.py",
        "res/enkf/run_arg.py",
        "res/enkf/plot/pca_fetcher.py",
        "res/enkf/runpath_list.py",
        "res/enkf/util/__init__.py",
        "res/enkf/queue_config.py",
        "res/enkf/summary_key_matcher.py",
        "res/enkf/summary_key_set.py",
        "res/fm/ecl/__init__.py",
        "res/enkf/state_map.py",
        "res/fm/ecl/script.py",
        "res/fm/rms/__init__.py",
        "res/fm/rms/rms_config.py",
        "res/enkf/site_config.py",
        "res/enkf/subst_config.py",
        "res/fm/shell/__init__.py",
        "res/fm/templating/__init__.py",
        "res/enkf/util/time_map.py",
        "res/job_queue/__init__.py",
        "res/fm/templating/template_render.py",
        "res/job_queue/environment_varlist.py",
        "res/fm/ecl/ecl_config.py",
        "res/job_queue/ert_plugin.py",
        "res/job_queue/driver.py",
        "res/job_queue/ext_joblist.py",
        "res/job_queue/external_ert_script.py",
        "res/fm/rms/rms_run.py",
        "res/fm/ecl/ecl_run.py",
        "res/job_queue/forward_model.py",
        "res/job_queue/ert_script.py",
        "res/job_queue/function_ert_script.py",
        "res/fm/shell/shell.py",
        "res/job_queue/job.py",
        "res/job_queue/run_status_type_enum.py",
        "res/job_queue/job_status_type_enum.py",
        "res/job_queue/ext_job.py",
        "res/job_queue/forward_model_status.py",
        "res/job_queue/job_queue_node.py",
        "res/job_queue/workflow_joblist.py",
        "res/job_queue/job_queue_manager.py",
        "res/job_queue/workflow.py",
        "res/sched/history_source_enum.py",
        "res/simulator/__init__.py",
        "res/job_queue/workflow_runner.py",
        "res/sched/history.py",
        "res/sched/sched_file.py",
        "res/test/synthesizer/__init__.py",
        "res/simulator/batch_simulator_context.py",
        "res/job_queue/queue.py",
        "res/simulator/batch_simulator.py",
        "res/enkf/res_config.py",
        "res/test/synthesizer/perlin.py",
        "res/test/ert_test_context.py",
        "res/util/enums/__init__.py",
        "res/job_queue/workflow_job.py",
        "res/util/enums/llsq_result_enum.py",
        "res/util/arg_pack.py",
        "res/test/synthesizer/prime_generator.py",
        "res/util/log.py",
        "res/util/enums/message_level_enum.py",
        "res/util/cthread_pool.py",
        "res/simulator/simulation_context.py",
        "res/util/res_log.py",
        "res/util/res_version.py",
        "res/util/path_format.py",
        "res/util/stat.py",
        "tests/__init__.py",
        "tests/conftest.py",
        "res/test/synthesizer/shaped_perlin.py",
        "tests/global/test_import.py",
        "tests/legacy/test_analysis.py",
        "tests/ctest_run.py",
        "tests/legacy/test_config.py",
        "res/util/substitution_list.py",
        "tests/legacy/test_sched.py",
        "tests/legacy/test_job_queue.py",
        "res/test/synthesizer/oil_simulator.py",
        "res/util/ui_return.py",
        "tests/res/analysis/test_options_enum.py",
        "tests/res/analysis/test_linalg.py",
        "tests/legacy/test_enkf.py",
        "tests/res/analysis/test_std_enkf.py",
        "tests/import_tester.py",
        "tests/res/analysis/test_std_enkf_debug.py",
        "res/util/matrix.py",
        "tests/res/enkf/data/test_enkf_config_node.py",
        "tests/res/enkf/data/test_enkf_node.py",
        "tests/res/enkf/data/test_field_config.py",
        "tests/res/enkf/data/test_gen_data.py",
        "tests/res/analysis/test_rml.py",
        "tests/res/enkf/data/test_gen_kw_config.py",
        "tests/res/analysis/test_analysis_module.py",
        "tests/res/enkf/data/test_custom_kw.py",
        "tests/res/enkf/data/test_custom_kw_config.py",
        "tests/res/enkf/data/test_gen_data_config.py",
        "tests/res/enkf/data/test_summary.py",
        "tests/res/enkf/export/test_arg_loader.py",
        "tests/res/enkf/data/test_gen_kw_config_equinor.py",
        "tests/res/enkf/export/test_gen_data_collector.py",
        "tests/res/enkf/export/test_custom_kw_collector.py",
        "tests/res/enkf/data/test_gen_kw.py",
        "tests/res/enkf/data/test_ext_param.py",
        "tests/res/enkf/export/test_misfit_collector.py",
        "tests/res/enkf/export/test_gen_data_observation_collector.py",
        "tests/res/enkf/plot/__init__.py",
        "tests/res/enkf/export/test_gen_kw_collector.py",
        "tests/res/enkf/export/test_summary_collector.py",
        "tests/res/enkf/test_analysis_iter_config.py",
        "tests/res/enkf/export/test_export_join.py",
        "tests/res/enkf/export/test_summary_observation_collector.py",
        "tests/res/enkf/test_active_list.py",
        "tests/res/enkf/test_data_kw_define.py",
        "tests/res/enkf/plot/test_plot_data.py",
        "tests/res/enkf/export/test_design_matrix.py",
        "tests/res/enkf/test_block_obs.py",
        "tests/res/enkf/test_analysis_config.py",
        "tests/res/enkf/test_enkf_fs.py",
        "tests/res/enkf/test_custom_kw_config_set.py",
        "tests/res/enkf/test_enkf_fs_manager2.py",
        "tests/res/enkf/test_enkf_fs_manager1.py",
        "tests/res/enkf/test_enkf_library.py",
        "tests/res/enkf/test_ecl_config.py",
        "tests/res/enkf/test_enkf_load_results_manually.py",
        "tests/res/enkf/test_enkf_runpath.py",
        "tests/res/enkf/test_ert_run_context.py",
        "tests/res/enkf/test_enkf_transfer_env.py",
        "tests/res/enkf/test_enkf_sim_model.py",
        "tests/res/config/test_config.py",
        "tests/res/enkf/test_ensemble_config.py",
        "tests/res/enkf/test_forward_load_context.py",
        "tests/res/enkf/test_ert_context.py",
        "tests/res/enkf/test_field_config.py",
        "tests/res/enkf/test_enkf.py",
        "tests/res/enkf/test_hook_workflow.py",
        "tests/res/enkf/test_gen_obs.py",
        "tests/res/enkf/test_es_update.py",
        "tests/res/enkf/test_ert_templates.py",
        "tests/res/enkf/test_local_obsdata_node.py",
        "tests/res/enkf/test_field_export.py",
        "tests/res/enkf/test_key_manager.py",
        "tests/res/enkf/test_labscale.py",
        "tests/res/enkf/test_local_dataset.py",
        "tests/res/enkf/test_enkf_obs.py",
        "tests/res/enkf/test_hook_manager.py",
        "tests/res/enkf/test_meas_block.py",
        "tests/res/enkf/test_local_config.py",
        "tests/res/enkf/test_meas_data.py",
        "tests/res/enkf/test_obs_block.py",
        "tests/res/enkf/test_queue_config.py",
        "tests/res/enkf/test_log_config.py",
        "tests/res/enkf/test_obs_data.py",
        "tests/res/enkf/test_run_context.py",
        "tests/res/enkf/test_model_config.py",
        "tests/res/enkf/test_runpath_list_dump.py",
        "tests/res/enkf/test_rng_config.py",
        "tests/res/enkf/test_runpath_list_ert.py",
        "tests/res/enkf/test_site_config.py",
        "tests/res/enkf/test_simulation_batch.py",
        "tests/res/enkf/test_summary_key_matcher.py",
        "tests/res/enkf/test_summary_obs.py",
        "tests/res/enkf/test_runpath_list.py",
        "tests/res/enkf/test_summary_key_set.py",
        "tests/res/enkf/test_update.py",
        "tests/res/enkf/test_state_map.py",
        "tests/res/enkf/test_workflow_list.py",
        "tests/res/enkf/test_subst_config.py",
        "tests/res/fm/test_fm_config.py",
        "tests/res/fm/test_rms_config.py",
        "tests/res/enkf/test_programmatic_res_config.py",
        "tests/res/job_queue/test_equinor_jobmanager.py",
        "tests/res/enkf/test_time_map.py",
        "tests/res/job_queue/test_ert_plugin.py",
        "tests/res/fm/test_ecl_config.py",
        "tests/res/job_queue/test_ert_script.py",
        "tests/res/fm/test_templating.py",
        "tests/res/job_queue/test_function_ert_script.py",
        "tests/res/job_queue/test_ext_job.py",
        "tests/res/fm/test_rms_run.py",
        "tests/res/fm/test_ecl_run.py",
        "tests/res/job_queue/test_job_queue_manager.py",
        "tests/res/job_queue/test_workflow_joblist.py",
        "tests/res/enkf/test_res_config.py",
        "tests/res/job_queue/test_job_queue.py",
        "tests/res/job_queue/test_workflow.py",
        "tests/res/sched/test_sched.py",
        "tests/res/job_queue/test_workflow_job.py",
        "tests/res/run/test_run.py",
        "tests/res/fm/test_shell.py",
        "tests/res/testcase/test_testcase.py",
        "tests/res/util/test_log.py",
        "tests/res/job_queue/test_workflow_runner.py",
        "tests/res/testcase/test_mini_config.py",
        "tests/res/testcase/test_testarea.py",
        "tests/res/util/test_path_fmt.py",
        "tests/res/simulator/test_simulation_context.py",
        "tests/res/job_queue/workflow_common.py",
        "tests/res/util/test_stat.py",
        "tests/res/util/test_res_log.py",
        "tests/res/util/test_substitution_list.py",
        "tests/utils/__init__.py",
        "tests/res/util/test_ui_return.py",
        "tests/share/test_synthesizer.py",
        "tests/res/job_queue/test_forward_model_formatted_print.py",
        "tests/res/util/test_matrix.py",
        "tests/res/simulator/test_batch_sim.py",
    ]
    return [os.path.join(root, fname) for fname in excluded_files]
