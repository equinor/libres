pipeline {
    agent { label 'si-build' }
    environment {
        WORKING_DIR = sh(script: 'mktemp -d', , returnStdout: true).trim()
    }
    stages {
        stage('setup') {
            steps {
                echo "Started with WORKING_DIR: ${WORKING_DIR}"
                sh 'sh testjenkins.sh setup'
            }
        }
	stage('build ecl') {
            steps {
                sh 'sh testjenkins.sh build_ecl'
            }
        }
	stage('build res') {
            steps {
                sh 'sh testjenkins.sh build_res'
            }
        }
	stage('run ctest') {
            steps {
                sh 'sh testjenkins.sh run_ctest'
            }
        }
	stage('run pytest') {
            steps {
                sh 'sh testjenkins.sh run_pytest'
            }
        }
    }
}


