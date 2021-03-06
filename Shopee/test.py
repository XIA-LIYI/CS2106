import json
import csv
import pandas

print("Loading the file")
with open("./contacts.json",'r') as load_f:
    load_dict = json.load(load_f)
print("Loading Completed")
print("Starting running")
result_dict = []
email_dict = {}
phone_dict = {}
order_dict = {}
for i in load_dict:
    if (i['Email'] in email_dict):
        index = email_dict[i['Email']]
    elif (i['Phone'] in phone_dict):
        index = phone_dict[i['Phone']]
    elif (i['OrderId'] in phone_dict):
        index = phone_dict[i['OrderId']]
    else:
        index = len(result_dict)
    if (index == len(result_dict)):
        add_on = {}
        add_on['order_list'] = [i['Id']]
        add_on['contacts'] = i['Contacts']

        result_dict = result_dict + [add_on]
    else:
        result_dict[index]['order_list'] = result_dict[index]['order_list'] + [i['Id']]
        result_dict[index]['contacts'] += i['Contacts']
    email_dict[i['Email']] = index
    phone_dict[i['Phone']] = index
    order_dict[i['OrderId']] = index
result = [None] * len(load_dict)
for i in result_dict:
    string = ""
    for j in range(len(i['order_list'])):
        string += str(i['order_list'][j])
        if (j != len(i['order_list']) - 1):
            string += "-"
    for j in i['order_list']:
        result[j] = [j, string + ", " + str(i['contacts'])]

print("Running completed")
df = pandas.DataFrame(result)
df.to_csv('submission.gzip', compression='gzip', index=False)
# with open("submission.csv","w") as csvfile: 
#     writer = csv.writer(csvfile)
    
#     writer.writerow(["ticket_id","ticket_trace/contact"])
#     index = 0
#     for i in range(0, len(result)):
#         index = i
#         element = result[i]
#         writer.writerow([i, element[0] + ", " + str(element[1])])
#         print(index)
#     writer.writerow([0, 0])
#     csvfile.close()
