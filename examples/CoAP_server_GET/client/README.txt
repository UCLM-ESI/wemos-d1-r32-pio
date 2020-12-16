Deps: virtualenv


###
# To start virtualenv
###########
virtualenv .
source bin/activate
pip install -r requirements.txt


###
# To run client
###########
python client.py



###
# To exit the virtualenv
###########
deactivate


