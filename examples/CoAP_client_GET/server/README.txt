Deps: virtualenv


###
# To start server inside a virtualenv
###########
virtualenv .
source bin/activate
pip install -r requirements.txt
python server.py



###
# To exit the virtualenv
###########
deactivate


