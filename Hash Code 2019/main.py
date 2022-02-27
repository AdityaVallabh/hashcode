class Image:

    def __init__(self, id, type, tags):
        self.id = id
        self.type = type
        self.tags = tags

    def __str__(self):
        return str(self.id)

    def combine(self, other):
        tags = self.tags.union(other.tags)
        id = '{} {}'.format(self, other)
        new_image = Image(id, 'H', tags)
        return new_image

def v_to_h(images):
    mod = None
    flag = False
    images = list(filter(lambda x: x.type == 'V', images))
    images = sorted(images, key=lambda x: len(x.tags), reverse=True)
    modded = []

    for i in range(int(len(images)/2)):
        one, two = images[i], images[len(images)-i-1]
        modded.append(one.combine(two))

    return modded

n = int(input())
images = []
tags_d = {}
for i in range(n):
    line = input().split()
    t, _, tags = line[0], line[1], line[2:]
    image = Image(i, t, set(tags))
    images.append(image)

modded = v_to_h(images)
images.extend(modded)
images = list(filter(lambda x: x.type == 'H', images))
for image in images:
    for tag in image.tags:
        if tag in tags_d:
            tags_d[tag].append(image)
        else:
            tags_d[tag] = [image]

res = ''
cnt = 0
taken = set()

k = 2000
s_images = sorted(images, key=lambda x: len(x.tags), reverse=True)

curr = s_images[0]
cnt += 1
taken.add(curr)
res += '{}\n'.format(curr)
for _ in range(k):
    maxi = None
    nxt = None
    flag = True
    # ss_images = s_images if flag else s_images[k]
    ss_images = []
    for ss in range(len(s_images)):
        if s_images[ss] not in taken:
            ss_images.append(s_images[ss])
            if len(ss_images) > k:
                break
    for image in ss_images:
        if image.type == 'H' and image not in taken and image != curr:
            comm = len(curr.tags.intersection(image.tags))
            other = min(len(image.tags), len(curr.tags))
            tmp = min(comm, other-comm)
            if maxi:
                if maxi < tmp:
                    maxi = tmp
                    nxt = image
            else :
                maxi = tmp
                nxt = image
    curr = nxt
    cnt += 1
    taken.add(curr)
    res += '{}\n'.format(curr)
    flag = not flag

# for tag in tags_d:
#     t_images = tags_d[tag]
#     t_images = sorted(t_images, key=lambda x: len(x.tags))
#     for image in t_images:
#         if image.type == 'H' and image not in taken:
#             cnt += 1
#             taken.add(image)
#             res += '{}\n'.format(image)

ctags = sorted(tags_d, key=lambda x: len(tags_d[x]), reverse=True)
for tag1 in ctags[:10]:
    for tag2 in ctags[:10]:
        if tag1 == tag2:
            break
        t_images = tags_d[tag1]
        t_images = sorted(t_images, key=lambda x: len(x.tags))
        for image in t_images:
            if image.type == 'H' and image not in taken and image in tags_d[tag2]:
                cnt += 1
                taken.add(image)
                res += '{}\n'.format(image)

for tag in ctags[10:]:
    t_images = tags_d[tag]
    t_images = sorted(t_images, key=lambda x: len(x.tags))
    for image in t_images:
        if image.type == 'H' and image not in taken:
            cnt += 1
            taken.add(image)
            res += '{}\n'.format(image)

res = str(cnt) + '\n' + res
print(res)
